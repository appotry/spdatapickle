/*
 * Copyright 2009 Stephen Liu
 * For license terms, see the file COPYING along with this library.
 */

#include <ctype.h>

#include "spdpcode.hpp"
#include "spdpsyntax.hpp"
#include "spdpname.hpp"

SP_DPCodeRender :: SP_DPCodeRender( SP_DPNameRender * nameRender )
{
	mNameRender = nameRender;
}

SP_DPCodeRender :: ~SP_DPCodeRender()
{
}

void SP_DPCodeRender :: generateHeader( SP_DPSyntaxTree * syntaxTree, FILE * writer )
{
	char filename[ 128 ] = { 0 };
	mNameRender->getFileName( syntaxTree->getName(), filename, sizeof( filename ) );

	fprintf( writer, "/* %s.hpp\n", filename );
	fprintf( writer, "   Generated by spxml2struct from %s\n", syntaxTree->getDefineFile() );
	fprintf( writer, "   Copyright (C) 2009, Stephen Liu. All Rights Reserved.\n" );
	fprintf( writer, "*/\n" );
	fprintf( writer, "\n" );

	fprintf( writer, "#ifndef __%s_hpp__\n", filename );
	fprintf( writer, "#define __%s_hpp__\n", filename );
	fprintf( writer, "\n" );
	fprintf( writer, "#ifdef __cplusplus\n" );
	fprintf( writer, "extern \"C\" {\n" );
	fprintf( writer, "#endif\n" );
	fprintf( writer, "\n" );

	fprintf( writer, "#include \"spdpmetainfo.hpp\"\n" );
	fprintf( writer, "\n" );

	generateMetaEnum( syntaxTree, writer );

	SP_DPSyntaxStructVector * slist = syntaxTree->getStructList();

	SP_DPSyntaxStructVector::iterator sit = slist->begin();

	for( ; slist->end() != sit; ++sit ) {
		generateStruct( &(*sit), writer );

		fprintf( writer, "\n" );
	}

	char name[ 128 ] = { 0 };
	mNameRender->getStructBaseName( syntaxTree->getName(), name, sizeof( name ) );

	fprintf( writer, "typedef struct tagSP_DPMetaInfo SP_DPMetaInfo_t;\n" );
	fprintf( writer, "extern SP_DPMetaInfo_t * g%sMetaInfo;\n", name );

	fprintf( writer, "\n" );
	fprintf( writer, "#ifdef __cplusplus\n" );
	fprintf( writer, "}\n" );
	fprintf( writer, "#endif\n" );

	fprintf( writer, "\n" );
	fprintf( writer, "#endif\n" );
	fprintf( writer, "\n" );
}

void SP_DPCodeRender :: generateStruct( SP_DPSyntaxStruct * structure, FILE * writer )
{
	char structName[ 128 ] = { 0 };

	mNameRender->getStructBaseName( structure->getName(), structName, sizeof( structName ) );

	fprintf( writer, "typedef struct tag%s {\n", structName );

	SP_DPSyntaxFieldVector * flist = structure->getFieldList();

	SP_DPSyntaxFieldVector::iterator fit = flist->begin();

	for( ; flist->end() != fit; ++fit ) {
		generateField( &(*fit), writer );

		if( '\0' != *(fit->getReferTo()) && flist->end() != ( fit + 1 ) ) {
			fprintf( writer, "\n" );
		}
	}

	fprintf( writer, "} %s_t;\n", structName );
}

void SP_DPCodeRender :: generateField( SP_DPSyntaxField * field, FILE * writer )
{
	char typeName[ 128 ] = { 0 }, fieldName[ 128 ] = { 0 };

	if( field->getArraySize() > 0 ) {
		fprintf( writer, "\t%s %s[%d];\n", mNameRender->getTypeName( field->getType(), typeName, sizeof( typeName ) ),
				mNameRender->getFieldName( field->getName(), fieldName, sizeof( fieldName ) ), field->getArraySize() );
	} else {
		fprintf( writer, "\t%s %s;\n", mNameRender->getTypeName( field->getType(), typeName, sizeof( typeName ) ),
				mNameRender->getFieldName( field->getName(), fieldName, sizeof( fieldName ) ) );
	}
}

void SP_DPCodeRender :: generateMetadata( SP_DPSyntaxTree * syntaxTree, FILE * writer )
{
	char filename[ 128 ] = { 0 };
	mNameRender->getFileName( syntaxTree->getName(), filename, sizeof( filename ) );

	fprintf( writer, "/* %s.cpp\n", filename );
	fprintf( writer, "   Generated by spxml2struct from %s\n", syntaxTree->getDefineFile() );
	fprintf( writer, "   Copyright (C) 2009, Stephen Liu. All Rights Reserved.\n" );
	fprintf( writer, "*/\n" );
	fprintf( writer, "\n" );

	fprintf( writer, "#include \"%s.hpp\"\n", filename );
	fprintf( writer, "#include \"spdpmetainfo.hpp\"\n" );
	fprintf( writer, "\n" );

	SP_DPSyntaxStructVector * slist = syntaxTree->getStructList();

	SP_DPSyntaxStructVector::iterator sit = slist->begin();
	for( ; slist->end() != sit; ++sit ) {
		generateMetaField( &(*sit), writer );
	}

	generateMetaInfo( syntaxTree, writer );
}

void SP_DPCodeRender :: generateMetaEnum( SP_DPSyntaxTree * syntaxTree, FILE * writer )
{
	SP_DPSyntaxStructVector * slist = syntaxTree->getStructList();

	SP_DPSyntaxStructVector::iterator sit = slist->begin();

	fprintf( writer, "enum {\n" );

	for( ; slist->end() != sit; ++sit ) {
		char name[ 128 ] = { 0 }, tmp[ 128 ] = { 0 };
		snprintf( name, sizeof( name ), "eType%s",
				mNameRender->getStructBaseName( sit->getName(), tmp, sizeof( tmp ) ) );

		if( slist->begin() == sit ) {
			fprintf( writer, "\t%s = eTypeSPDPUserDefine + 1", name );
		} else {
			fprintf( writer, "\t%s", name );
		}

		if( slist->end() == ( sit + 1 ) ) {
			fprintf( writer, "\n" );
		} else {
			fprintf( writer, ",\n" );
		}
	}

	fprintf( writer, "};\n" );
	fprintf( writer, "\n" );
}

void SP_DPCodeRender :: generateMetaField( SP_DPSyntaxStruct * structure, FILE * writer )
{
	char sname[ 128 ] = { 0 }, fname[ 128 ] = { 0 }, ename[ 128 ] = { 0 };
	char tname[ 128 ] = { 0 }, rawname[ 128 ] = { 0 };

	fprintf( writer, "static SP_DPMetaField_t gMeta%s [] = {\n",
			mNameRender->getStructBaseName( structure->getName(), sname, sizeof( sname ) ) );

	SP_DPSyntaxFieldVector * flist = structure->getFieldList();

	SP_DPSyntaxFieldVector::iterator fit = flist->begin();

	for( ; flist->end() != fit; ++fit ) {
		mNameRender->getFieldName( fit->getName(), fname, sizeof( fname ) );
		mNameRender->getTypeEnum( fit->getType(), ename, sizeof( ename ) );

		mNameRender->getTypeFullName( &(*fit), tname, sizeof( tname ) );
		mNameRender->getTypeRawName( fit->getType(), rawname, sizeof( rawname ) );

		int isPtr = fit->isPtr();
		if( 0 == isPtr ) isPtr = fit->getArraySize() > 0;

		fprintf( writer, "\t{ \"%s\", SP_DP_FIELD_OFFSET(%s_t, %s), %s, %d, %d,\n"
				"\t\t\"%s\", sizeof(%s), sizeof(%s) }",
				fit->getName(), sname, fname, ename, isPtr ? 1 : 0,
				fit->getArraySize(), fit->getReferTo(), tname, rawname );

		if( flist->end() == ( fit + 1 ) ) {
			fprintf( writer, "\n" );
		} else {
			fprintf( writer, ",\n" );
		}
	}

	fprintf( writer, "};\n" );

	fprintf( writer, "\n" );
}

void SP_DPCodeRender :: generateMetaInfo( SP_DPSyntaxTree * syntaxTree, FILE * writer )
{
	SP_DPSyntaxStructVector * slist = syntaxTree->getStructList();

	SP_DPSyntaxStructVector::iterator sit = slist->begin();

	fprintf( writer, "static SP_DPMetaStruct_t gMeta%sStructList [] = {\n",
			syntaxTree->getPrefix() );

	for( ; slist->end() != sit; ++sit ) {
		char ename[ 129 ] = { 0 }, sname[ 128 ] = { 0 };
		mNameRender->getTypeEnum( sit->getName(), ename, sizeof( ename ) );
		mNameRender->getStructBaseName( sit->getName(), sname, sizeof( sname ) );

		fprintf( writer, "\t{ %s, \"%s\", sizeof(%s_t),\n"
				"\t\tSP_DP_ARRAY_SIZE(gMeta%s), gMeta%s }",
				ename, sit->getName(), sname, sname, sname );

		if( slist->end() == ( sit + 1 ) ) {
			fprintf( writer, "\n" );
		} else {
			fprintf( writer, ",\n" );
		}
	}

	fprintf( writer, "};\n" );
	fprintf( writer, "\n" );

	char name[ 128 ] = { 0 };
	mNameRender->getStructBaseName( syntaxTree->getName(), name, sizeof( name ) );

	const char * prefix = syntaxTree->getPrefix();

	fprintf( writer, "static SP_DPMetaInfo_t gMeta%s = {\n", name );
	fprintf( writer, "\t\"%s\", \"%s\", SP_DP_ARRAY_SIZE(gMeta%sStructList), gMeta%sStructList\n",
			prefix, syntaxTree->getName(), prefix, prefix );
	fprintf( writer, "};\n" );

	fprintf( writer, "\n" );

	fprintf( writer, "SP_DPMetaInfo_t * g%sMetaInfo = &gMeta%s;\n", name, name );

	fprintf( writer, "\n" );
}

