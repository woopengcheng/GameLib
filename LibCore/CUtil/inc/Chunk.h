#ifndef __cutil_chunk_h__
#define __cutil_chunk_h__ 
#include "CUtil/inc/Common.h" 
#include "CUtil/inc/ReferCount.h" 

namespace CUtil
{ 
	class DLL_EXPORT ChunkData
	{ 
	public:
		explicit ChunkData(INT32 nRefCount = 0)
			: m_unSize(0)
			, m_unDataLen(0)
			, m_refCount(nRefCount)
		{}
	public:
		static ChunkData	*	Create(UINT32 unSize);
		static void			*	operator new (size_t size , UINT32  extra);
		static void				operator delete (void *p , UINT32  extra);
		static ChunkData	&	Null();

	public:
		bool			operator !=(const ChunkData & objChunk) const;
		bool			operator ==(const ChunkData & objChunk) const; 

	public:
		void		*	Insert(void * pPos , void * pBegin , UINT32 unLen);
		void		*	Pushback (void * pBegin , UINT32 unLen); 
		ChunkData	&	Erase(void * pBegin , void * pEnd);
		ChunkData	*	Reverse(UINT32 unSize);
		void		*	Begin( void );
		void		*	End( void );
		void			Clear( void );
		void			AddRef(void);
		void			DecRef( void );

	public:
		void		*	GetData() { return reinterpret_cast<void *>(this + 1); } 
		void		*	GetData() const { return reinterpret_cast<void *>(const_cast<ChunkData*>(this) + 1); } 
		UINT32			GetSize() const { return m_unSize; } 
		UINT32			GetDataLen() const { return m_unDataLen; }  
		void			SetDataLen(UINT32 unDataLen) { m_unDataLen = unDataLen; }  
		INT32			GetRefCount(void) const { return m_refCount; }

	private:
		UINT32			FitSize(UINT32 unSize);

	public:
		INT32			m_refCount;
		UINT32			m_unSize;
		UINT32			m_unDataLen; 
	};

	class DLL_EXPORT Chunk
	{
	public: 
		Chunk()
			: m_pData(ChunkData::Null().GetData())
		{
			GetChunkData()->AddRef();
		}
		Chunk(UINT32 unChunkSize)
			: m_pData(ChunkData::Create(unChunkSize)->GetData())
		{}
		Chunk(const void * pBuf , UINT32 unChunkSize , bool bRelease = true)  //5 使用这个接口的时候要特别注意.长度一定要大于chunkdata,而且不要有数据
		{
			if (bRelease)
			{
				m_pData = ChunkData::Create(unChunkSize)->GetData();

				memcpy(m_pData , pBuf , unChunkSize);
				GetChunkData()->SetDataLen(unChunkSize);
			}
			else
			{
				if ( unChunkSize >= sizeof(ChunkData))
				{
					ChunkData * pData = ((ChunkData *)(pBuf));
					pData->m_refCount = -1;
					pData->m_unDataLen = 0;
					pData->m_unSize = unChunkSize - sizeof(CUtil::ChunkData); 
					m_pData = pData->GetData();
				}
			}
		}
		Chunk(const Chunk & objChunk);
		~Chunk()
		{
			GetChunkData()->DecRef();
		}
		
		ChunkData	*		GetChunkData(){ return reinterpret_cast<ChunkData *>(m_pData) - 1; }
		ChunkData	*		GetChunkData() const { return reinterpret_cast<ChunkData *>(m_pData) - 1; }

	public:
		Chunk		&		operator = (const Chunk & objChunk);
		bool				operator !=(const Chunk & objChunk) const;
		bool				operator ==(const Chunk & objChunk) const;

	public:
		virtual Chunk	&	Insert(void * pPos , void * pBegin , UINT32 unLen);
		virtual Chunk   &	Pushback (void * pBegin , UINT32 unLen); 
		virtual Chunk   &	Erase(void * pBegin , void * pEnd);
		virtual Chunk   &	Reverse(UINT32 unSize); 
		virtual void    *	Begin( void );
		virtual void    *	Begin( void ) const;
		virtual void    *	End( void );
		virtual void    *	End( void ) const ;
		virtual void		Clear( void );

	public: 
		UINT32				GetSize() const { return GetChunkData()->GetSize(); } 
		UINT32				GetDataLen() const { return GetChunkData()->GetDataLen(); }
		friend	std::ostream&	operator<<(std::ostream&os,const Chunk & objChunk);

	protected:
		void			*	m_pData;
	};

	template<UINT32 unSize> 
	class StackChunk : public Chunk
	{ 
	public:  
		StackChunk()
			: Chunk(m_szBuffer , unSize , false)
		{ 
		}

	private:
		char  m_szBuffer[unSize];
	};
}
#define CUtilChunk CUtil::Chunk
#endif
