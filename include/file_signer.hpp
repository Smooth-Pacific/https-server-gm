#ifndef FILE_SIGNER_HEADER
#define FILE_SIGNER_HEADER

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <xmlsec/xmlsec.h>
#include <xmlsec/xmltree.h>
#include <xmlsec/xmldsig.h>
#include <xmlsec/xmlenc.h>
#include <xmlsec/templates.h>
#include <xmlsec/crypto.h>

namespace file_signer {
    int sign_file(const char* xml_file, const char* key_file, const char* outfile);
    int signXML(const char* filename, const char* outfile);
}

#endif