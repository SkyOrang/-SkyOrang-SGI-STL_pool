#include"pool_b.h"

namespace xzh
{
	using std::cout;
	using std::endl;
	using std::vector;

	class Memory_Pool
	{
	public:
		Memory_Pool(size_t num = 8*1024*1024)
		{
			_ptr.push_back(nullptr);
			_ptr[0] = malloc(num);
			_ncap = num;
			_usptr = _ptr[0];
			_eptr = (char*)_ptr[0] + _ncap;
			for (int i = 0; i < 16; i++)
			{
				_mm.push_back(nullptr);
			}
			
		}
		size_t GetIndex(size_t num)
		{
			int index = num / 8;
			if (index * 8 < num)
			{
				index += 1;
			}
			return index;
		}

		void* Malloc(size_t num)
		{
			size_t index = GetIndex(num);
			if (_mm[index - 1] == nullptr)
			{
				if ((char*)_usptr + index * 8 + sizeof(void*) > _eptr)
				{
					size_t size = _ptr.size();
					_ptr.push_back(nullptr);
					void* p = malloc(_ncap * 2);
					_ptr[size] = p;
					_usptr = _ptr[size];
					_ncap *= 2;
					_eptr = (char*)_ptr[size] + _ncap;
					_usptr = (char*)_usptr + index * 8 + sizeof(void*);
					void** pr = (void**)((char*)_usptr - sizeof(void*));
					*pr = nullptr;
					return _ptr[size];
				}
				else
				{
					void* ptr = _usptr;
					_usptr = (char*)_usptr + index * 8 + sizeof(void*);
					void** pr = (void**)((char*)_usptr - sizeof(void*));
					*pr = nullptr;
					return ptr;
				}

			}
			else
			{
				char* ptr = (char*)_mm[index - 1];
				void** pr = (void**)(ptr + (index * 8));
				void* p = *pr;

				_mm[index - 1] = p;
				return (void*)ptr;
			}



		}
		void Free(size_t size, void* pr)
		{
			size_t index = GetIndex(size);
			void* ptr = _mm[index - 1];
			_mm[index - 1] = pr;
			void** p = (void**)((char*)pr + (index * 8));
			*p = ptr;
		}

		~Memory_Pool()
		{
			for (auto e : _ptr)
			{
				free(e);
			}
		}
	private:
		std::vector<void*> _ptr;
		void* _usptr;
		void* _eptr;
		size_t _ncap;
		std::vector<void*> _mm;

	};

	Memory_Pool p;


	void* Malloc(size_t num)
	{
		if (num > 128)
		{
			return malloc(num);
		}
		else
		{
			return p.Malloc(num);

		}

	}


	void Free(void* ptr,size_t size)
	{
		if (size > 128)
		{
			free(ptr);
		}
		else
		{
			p.Free(size,ptr);
		}
	}
	
}