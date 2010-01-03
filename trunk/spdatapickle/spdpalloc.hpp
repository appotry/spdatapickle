/*
 * Copyright 2009 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#ifndef __spdpalloc_hpp__
#define __spdpalloc_hpp__

typedef struct tagSP_DPMetaInfo SP_DPMetaInfo_t;
typedef struct tagSP_DPMetaStruct SP_DPMetaStruct_t;
typedef struct tagSP_DPMetaField SP_DPMetaField_t;

class SP_DPAlloc
{
public:
	SP_DPAlloc( SP_DPMetaInfo_t * metaInfo );
	~SP_DPAlloc();

	int free( void * structure, int size, int type );

	int deepCopy( const void * src, int srcSize, int type, void * dest, int destSize );

private:
	int copyBaseType( const void * src, int srcSize, SP_DPMetaStruct_t * metaStruct,
			SP_DPMetaField_t * field, void * dest, int destSize );

private:
	SP_DPMetaInfo_t * mMetaInfo;
};

#endif

