/*
 * Copyright 2009 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <string.h>

#include "spdpsyntax.hpp"

#define SP_DP_MEMSET(s)   memset(s,0,sizeof(s))
#define SP_DP_STRCPY(d,s) strncpy(d,s,sizeof(d)-1)

SP_DPSyntaxNode :: SP_DPSyntaxNode()
{
	SP_DP_MEMSET(mName);
}

SP_DPSyntaxNode :: ~SP_DPSyntaxNode()
{
}

void SP_DPSyntaxNode :: setName( const char * name )
{
	SP_DP_STRCPY( mName, name );
}

const char * SP_DPSyntaxNode :: getName() const
{
	return mName;
}

//=========================================================

SP_DPSyntaxField :: SP_DPSyntaxField()
{
	mId = 0;
	mIsRequired = 1;

	SP_DP_MEMSET(mType);
	SP_DP_MEMSET(mReferTo);
	mArraySize = 0;
}

SP_DPSyntaxField :: ~SP_DPSyntaxField()
{
}

void SP_DPSyntaxField :: setId( int id )
{
	mId = id;
}

int SP_DPSyntaxField :: getId()
{
	return mId;
}

void SP_DPSyntaxField :: setType( const char * type )
{
	SP_DP_STRCPY(mType,type);
}

const char * SP_DPSyntaxField :: getType() const
{
	return mType;
}

int SP_DPSyntaxField :: isPtr() const
{
	return '*' == mType[0];
}

void SP_DPSyntaxField :: setArraySize( int arraySize )
{
	mArraySize = arraySize;
}

int SP_DPSyntaxField :: getArraySize() const
{
	return mArraySize;
}

void SP_DPSyntaxField :: setReferTo( const char * referTo )
{
	SP_DP_STRCPY(mReferTo,referTo);
}

const char * SP_DPSyntaxField :: getReferTo() const
{
	return mReferTo;
}

void SP_DPSyntaxField :: setRequired( int isRequired )
{
	mIsRequired = isRequired;
}

int  SP_DPSyntaxField :: isRequired()
{
	return mIsRequired;
}

//=========================================================

SP_DPSyntaxStruct :: SP_DPSyntaxStruct()
{
}

SP_DPSyntaxStruct :: ~SP_DPSyntaxStruct()
{
}

SP_DPSyntaxFieldVector * SP_DPSyntaxStruct :: getFieldList()
{
	return &mFieldList;
}

//=========================================================

SP_DPSyntaxTree :: SP_DPSyntaxTree()
{
	SP_DP_MEMSET(mPrefix);
	SP_DP_MEMSET(mDefineFile);
}

SP_DPSyntaxTree :: ~SP_DPSyntaxTree()
{
}

void SP_DPSyntaxTree :: setDefineFile( const char * defineFile )
{
	SP_DP_STRCPY( mDefineFile, defineFile );
}

const char * SP_DPSyntaxTree :: getDefineFile() const
{
	return mDefineFile;
}

void SP_DPSyntaxTree :: setPrefix( const char * prefix )
{
	SP_DP_STRCPY( mPrefix, prefix );
}

const char * SP_DPSyntaxTree :: getPrefix() const
{
	return mPrefix;
}

SP_DPSyntaxStructVector * SP_DPSyntaxTree :: getStructList()
{
	return &mStructList;
}

