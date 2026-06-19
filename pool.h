#pragma once
#include"pool_b.h"

//为STL容器提供内存分配支持
namespace xzh
{
	template <typename T>
	struct PoolAllocator {
		using value_type = T;

		PoolAllocator() = default;

		template <typename U>
		PoolAllocator(const PoolAllocator<U>&) noexcept
		{

		}

		T* allocate(std::size_t n)
		{

			return (T*)xzh::Malloc(sizeof(T) * n);
		}

		void deallocate(T* ptr, std::size_t n) noexcept
		{
			xzh::Free(ptr, sizeof(T) * n);
		}
	};

	template <typename T, typename U>
	bool operator==(const PoolAllocator<T>&, const PoolAllocator<U>&) {
		return true;
	}

	template <typename T, typename U>
	bool operator!=(const PoolAllocator<T>&, const PoolAllocator<U>&) {
		return false;
	}
}

//有默认构造函数时可以使用
namespace xzh
{

#define DataType(T) (Data<T>()) 

	struct Normal
	{

	};

	struct Special
	{

	};

	struct Array
	{

	};

	template<class T>
	struct Data
	{
		T _d;
	};

	template<class T,class IsSpecial=Normal>
	class pool_ptr
	{
	public:
		pool_ptr();
		
		pool_ptr(Data<T>* data);
		
		pool_ptr<T, IsSpecial>& operator=(Data<T>* data);
		
		bool operator==(const pool_ptr<T, IsSpecial>& ptr);
		
		bool operator!=(const pool_ptr<T, IsSpecial>& ptr);
		
		T& operator*();
		
		T* operator->();
		
		Data<T>* GetData() const;

		T* GetPtr();

		
	public:
		Data<T>* _d;
	};
	
	template<class T, class IsSpecial>
	pool_ptr<T, IsSpecial>::pool_ptr()
	{
		_d = nullptr;
	}
	template<class T, class IsSpecial>
	pool_ptr<T, IsSpecial>::pool_ptr(Data<T>* data)
	{
		_d = data;
	}
	template<class T, class IsSpecial>
	pool_ptr<T, IsSpecial>& pool_ptr<T, IsSpecial>::operator=(Data<T>* data)
	{
		_d = data;
		return *this;
	}
	template<class T, class IsSpecial>
	bool pool_ptr<T, IsSpecial>::operator==(const pool_ptr<T, IsSpecial>& ptr)
	{
		return(_d == ptr._d);
	}
	template<class T, class IsSpecial>
	bool pool_ptr<T, IsSpecial>::operator!=(const pool_ptr<T, IsSpecial>& ptr)
	{

		return!(_d == ptr._d);
	}
	template<class T, class IsSpecial>
	T& pool_ptr<T, IsSpecial>::operator*()
	{
		return _d->_d;
	}
	template<class T, class IsSpecial>
	T* pool_ptr<T, IsSpecial>::operator->()
	{
		return &(_d->_d);
	}
	template<class T, class IsSpecial>
	Data<T>* pool_ptr<T, IsSpecial>::GetData() const
	{
		return _d;
	}

	template<class T, class IsSpecial>
	T* pool_ptr<T, IsSpecial>::GetPtr() 
	{
		return &(_d->_d);
	}

	template<class T>
	Data<T>* PoolNew(const Data<T>&);

	template<class T>
	void PoolDelete(Data<T>* ptr);

	template<class T>
	void PoolDelete(const pool_ptr<T,Normal>& pr);

	template<class T>
	Data<T>* PoolNew(const Data<T>&)
	{
		void*re = Malloc(sizeof(Data<T>));
		Data<T>* pr = (Data<T>*)re;
		new(&(pr->_d)) T();
		return pr;
	}

	template<class T>
	void PoolDelete(Data<T>* ptr)
	{
		ptr->_d.T::~T();
		xzh::Free(ptr, sizeof((*ptr)));
	}

	template<class T>
	void PoolDelete(const pool_ptr<T,Normal>& pr)
	{
		Data<T>* ptr = pr.GetData();
		ptr->_d.T::~T();
		xzh::Free(ptr, sizeof((*ptr)));
	}

	template<class T>
	Data<T>* PoolMalloc(const Data<T>&);

	template<class T>
	void PoolFree(Data<T>* ptr);

	template<class T>
	void PoolFree(const pool_ptr<T, Normal>& pr);

	template<class T>
	Data<T>* PoolMalloc(const Data<T>&)
	{
		void* re = Malloc(sizeof(Data<T>));
		Data<T>* pr = (Data<T>*)re;
		return pr;
	}

	template<class T>
	void PoolFree(Data<T>* ptr)
	{
		xzh::Free(ptr, sizeof((*ptr)));
	}

	template<class T>
	void PoolFree(pool_ptr<T, Normal>& pr)
	{
		Data<T>* ptr = pr.GetData();
		xzh::Free(ptr, sizeof((*ptr)));
	}
}

//要创建数组可以使用
namespace xzh
{
#define ADataType(T) (AData<T>()) 
	template<class T>
	struct AData
	{
		T* GetPtr()
		{
			return (T*)((char*)this + sizeof(AData<T>));
		}
		size_t _num;
	};

	template<class T>
	class pool_ptr<T, Array>
	{
	public:
		pool_ptr();

		pool_ptr(AData<T>* data);

		pool_ptr<T, Array>& operator=(AData<T>* data);

		bool operator==(const pool_ptr<T, Array>& ptr);

		bool operator!=(const pool_ptr<T, Array>& ptr);

		T& operator*();

		T* operator->();

		AData<T>* GetData() const;

		T* GetPtr();


	private:
		AData<T>* _d;
	};

	template<class T>
	pool_ptr<T, Array>::pool_ptr()
	{
		_d = nullptr;
	}
	template<class T>
	pool_ptr<T, Array>::pool_ptr(AData<T>* data)
	{
		_d = data;
	}
	template<class T>
	pool_ptr<T, Array>& pool_ptr<T, Array>::operator=(AData<T>* data)
	{
		_d = data;
		return *this;
	}
	template<class T>
	bool pool_ptr<T, Array>::operator==(const pool_ptr<T, Array>& ptr)
	{
		return(_d == ptr._d);
	}
	template<class T>
	bool pool_ptr<T, Array>::operator!=(const pool_ptr<T, Array>& ptr)
	{

		return!(_d == ptr._d);
	}
	template<class T>
	T& pool_ptr<T, Array>::operator*()
	{
		T* pr = _d->GetPtr();
		return *pr;
	}
	template<class T>
	T* pool_ptr<T, Array>::operator->()
	{
		return (_d->GetPtr());
	}
	template<class T>
	AData<T>* pool_ptr<T, Array>::GetData() const
	{
		return _d;
	}

	template<class T>
	T* pool_ptr<T, Array>::GetPtr() 
	{
		return _d->GetPtr();
	}

	template<class T>
	AData<T>* PoolNew(const AData<T>&, size_t num);

	template<class T>
	void PoolDelete(AData<T>* ptr);

	template<class T>
	void PoolDelete(const pool_ptr<T, Array>& pr);

	template<class T>
	AData<T>* PoolNew(const AData<T>&, size_t num)
	{
		void* re = Malloc(sizeof(AData<T>)+sizeof(T) * num);
		AData<T>* pr = (AData<T>*)((char*)re + sizeof(AData<T>));
		T* p = (T*)(pr);
		for (int i = 0; i < num; i++)
		{
			new (p+i) T();
		}

		((AData<T>*)re)->_num = num;
		return (AData<T>*)re;
	}

	template<class T>
	void PoolDelete(AData<T>* ptr)
	{
		size_t num = ptr->_num;
		T* p = (T*)((char*)ptr+sizeof(AData<T>));
		for (int i = 0; i < num; i++)
		{
			(p + i)->~T();
		}
		xzh::Free(ptr, sizeof(T)*num+sizeof(AData<T>));
	}

	template<class T>
	void PoolDelete(const pool_ptr<T, Array>& pr)
	{
		AData<T>* ptr = pr.GetData();
		size_t num = ptr->_num;
		for (int i = 0; i < num; i++)
		{
			((ptr->GetPtr()) + i)->~T();
		}
		xzh::Free(ptr, sizeof(T) * num + sizeof(AData<T>));
	}

	template<class T>
	AData<T>* PoolMalloc(const AData<T>&, size_t num);

	template<class T>
	void PoolFree(AData<T>* ptr);

	template<class T>
	void PoolFree(const pool_ptr<T, Array>& pr);

	template<class T>
	AData<T>* PoolMalloc(const AData<T>&, size_t num)
	{
		void* re = Malloc(sizeof(AData<T>) + sizeof(T) * num);
	
		((AData<T>*)re)->_num = num;
		return (AData<T>*)re;
	}

	template<class T>
	void PoolFree(AData<T>* ptr)
	{
		size_t num = ptr->_num;
		
		xzh::Free(ptr, sizeof(T) * num + sizeof(AData<T>));
	}

	template<class T>
	void PoolFree(pool_ptr<T, Array>& pr)
	{
		AData<T>* ptr = pr.GetData();
		size_t num = ptr->_num;
		
		xzh::Free(ptr, sizeof(T) * num + sizeof(AData<T>));
	}
}

//没有默认构造函数时可以使用
namespace xzh
{
#define SDataType(T) (SData<T>()) 
	template<class T>
	struct SData
	{

	};


	template<class T>
	class pool_ptr<T, Special>
	{
	public:
		pool_ptr();

		pool_ptr(SData<T>* data);

		pool_ptr<T, Special>& operator=(SData<T>* data);

		bool operator==(const pool_ptr<T, Special>& ptr);

		bool operator!=(const pool_ptr<T, Special>& ptr);

		T& operator*();

		T* operator->();

		SData<T>* GetData() const;

		T* GetPtr();

	private:
		SData<T>* _d;
	};

	template<class T>
	pool_ptr<T, Special>::pool_ptr()
	{
		_d = nullptr;
	}
	template<class T>
	pool_ptr<T, Special>::pool_ptr(SData<T>* data)
	{
		_d = data;
	}
	template<class T>
	pool_ptr<T, Special>& pool_ptr<T, Special>::operator=(SData<T>* data)
	{
		_d = data;
		return *this;
	}
	template<class T>
	bool pool_ptr<T, Special>::operator==(const pool_ptr<T, Special>& ptr)
	{
		return(_d == ptr._d);
	}
	template<class T>
	bool pool_ptr<T, Special>::operator!=(const pool_ptr<T, Special>& ptr)
	{

		return!(_d == ptr._d);
	}
	template<class T>
	T& pool_ptr<T, Special>::operator*()
	{
		T* pr = (T*)_d;
		return *pr;
	}
	template<class T>
	T* pool_ptr<T, Special>::operator->()
	{
		return (T*)_d;
	}
	template<class T>
	SData<T>* pool_ptr<T, Special>::GetData() const
	{
		return _d;
	}

	template<class T>
	T* pool_ptr<T, Special>::GetPtr()
	{
		return (T*)_d;
	}

	template<class T>
	SData<T>* PoolMalloc(const SData<T>&);

	template<class T>
	void PoolFree(SData<T>* ptr);

	template<class T>
	void PoolFree(pool_ptr<T, Special>& pr);

	template<class T>
	SData<T>* PoolMalloc(const SData<T>&)
	{
		void* re = Malloc(sizeof(T));
		SData<T>* pr = (SData<T>*)((char*)re + sizeof(SData<T>));
		return pr;
	}

	template<class T>
	void PoolFree(SData<T>* ptr)
	{
		xzh::Free(ptr , sizeof(T));
	}

	template<class T>
	void PoolFree(pool_ptr<T, Special>& pr)
	{
		SData<T>* ptr = (SData<T>*)(pr.GetData());

		xzh::Free(ptr, sizeof(T));
	}
}

using xzh::Data;
using xzh::AData;
using xzh::SData;
using xzh::Normal;
using xzh::Array;
using xzh::Special;