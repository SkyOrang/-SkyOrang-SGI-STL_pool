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
				if ((char*)_usptr +8+ index * 8 + sizeof(void*) > _eptr)
				{
					size_t size = _ptr.size();
					_ptr.push_back(nullptr);
					void* p = malloc(_ncap);
					_ptr[size] = p;
					_usptr = _ptr[size];
					*((size_t*)_usptr) = index;
					_eptr = (char*)_ptr[size] + _ncap;
					_usptr = (char*)_usptr + 8 + index * 8 + sizeof(void*);
					void** pr = (void**)((char*)_usptr-sizeof(void*));
					*pr = nullptr;
					return _ptr[size];
				}
				else
				{
					char* ptr = (char*)_usptr;
					*ptr = (unsigned int)index;
					_usptr = (char*)_usptr + 8 + index * 8 + sizeof(void*);
					void** pr = (void**)((char*)_usptr - sizeof(void*));
					*pr = nullptr;
					return ptr + 8;
				}

			}
			else
			{
				char* ptr = (char*)_mm[index - 1];
				void** pr = (void**)(ptr + 8 + (index * 8));
				void* p = *pr;

				_mm[index - 1] = p;
				return (void*)(ptr + 8);
			}


		}
		void* SpecialMalloc(size_t num)
		{
			size_t index = GetIndex(num);

			if ((char*)_usptr + 8 + index * 8 + sizeof(void*) > _eptr)
			{
				size_t size = _ptr.size();
				_ptr.push_back(nullptr);
				void* p = malloc(_ncap);
				_ptr[size] = p;
				_usptr = _ptr[size];
				*((size_t*)_usptr) = index;
				_eptr = (char*)_ptr[size] + _ncap;
				_usptr = (char*)_usptr + 8 + index * 8 + sizeof(void*);
				void** pr = (void**)((char*)_usptr - sizeof(void*));
				*pr = nullptr;
				return _ptr[size];
			}
			else
			{
				char* ptr = (char*)_usptr;
				*ptr = (unsigned int)index;
				_usptr = (char*)_usptr + 8 + index * 8 + sizeof(void*);
				void** pr = (void**)((char*)_usptr - sizeof(void*));
				*pr = nullptr;
				return ptr + 8;
			}

		}
		void Free(void* ptr)
		{
			size_t index = (*(char*)((char*)ptr - 8));
			if (index > 16)
			{
				size_t count = index * 8+16;
				size_t m = count;
				char* nptr = (char*)ptr - 8;
				while (1)
				{
					if (m-144-24 >= 0)
					{
						(*(char*)((char*)ptr - 8)) = 16;
						void** ppr = (void**)((char*)ptr + 16 * 8);
						*ppr = nullptr;
						void* pr = _mm[16 - 1];
						_mm[16 - 1] = ((char*)ptr - 8);
						*ppr = pr;
						m -= 144;
						nptr += 144;

					}
					else
					{
						size_t nindex = GetIndex(m - 16);
						if (nindex == 17)
						{
							nindex = 16;
						}
						void** ppr = (void**)((char*)nptr +8+ nindex * 8);
						*ppr = nullptr;
						void* pr = _mm[nindex - 1];
						_mm[nindex - 1] = ((char*)nptr);
						*ppr = pr;
						break;
					}
				}
				

			}
			void* pr = _mm[index - 1];
			_mm[index - 1] = ((char*)ptr - 8);
			void** p = (void**)((char*)ptr + (index * 8));
			*p = pr;
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
			p.Free(ptr);
		}
	}
	
}