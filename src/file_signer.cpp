#include "../include/file_signer.hpp"


int file_signer::sign_file(const char* xml_file, const char* key_file, const char* outfile) {
    xmlDocPtr doc = NULL;
    xmlNodePtr signNode = NULL;
    xmlNodePtr refNode = NULL;
    xmlNodePtr keyInfoNode = NULL;
    xmlSecDSigCtxPtr dsigCtx = NULL;
    
    
    /* load doc file */
    doc = xmlParseFile(xml_file);
    if ((doc == NULL) || (xmlDocGetRootElement(doc) == NULL)){
        return -1;      
    }
    /* create signature template for RSA-SHA1 enveloped signature */
    signNode = xmlSecTmplSignatureCreate(doc, xmlSecTransformExclC14NId, xmlSecTransformRsaSha1Id, NULL);
    /* add <dsig:Signature/> node to the doc */
    xmlAddChild(xmlDocGetRootElement(doc), signNode);
    /* add reference */
    refNode = xmlSecTmplSignatureAddReference(signNode, xmlSecTransformSha1Id, NULL, NULL, NULL);
    /* add enveloped transform */
    if(xmlSecTmplReferenceAddTransform(refNode, xmlSecTransformEnvelopedId) == NULL) {
    	xmlFreeDoc(doc); 
        return -1;              
    }
    /* add <dsig:KeyInfo/> and <dsig:KeyName/> nodes to put key name in the signed document */
    keyInfoNode = xmlSecTmplSignatureEnsureKeyInfo(signNode, NULL);
    if(xmlSecTmplKeyInfoAddKeyName(keyInfoNode, NULL) == NULL) {
    	xmlFreeDoc(doc); 
        return -1;              
    }
    /* create signature context */
    dsigCtx = xmlSecDSigCtxCreate(NULL);
    /* load private key, assuming that there is not password */
    dsigCtx->signKey = xmlSecCryptoAppKeyLoad(key_file, xmlSecKeyDataFormatPem, NULL, NULL, NULL);
    if(xmlSecDSigCtxSign(dsigCtx, signNode) < 0) {
    	if(dsigCtx != NULL) {
        	xmlSecDSigCtxDestroy(dsigCtx);
    	}
        xmlFreeDoc(doc); 
        return -1;
    }
    xmlSaveFormatFileEnc(outfile, doc, "UTF-8", 1);

    /* cleanup */
    if(dsigCtx != NULL) {
        xmlSecDSigCtxDestroy(dsigCtx);
    }
    if(doc != NULL) {
        xmlFreeDoc(doc); 
    }
    return 0;
}

int file_signer::signXML(const char* filename, const char* outfile) {
	/* Init libxml and libxslt libraries */
    xmlInitParser();
    LIBXML_TEST_VERSION
    xmlLoadExtDtdDefaultValue = XML_DETECT_IDS | XML_COMPLETE_ATTRS;
    xmlSubstituteEntitiesDefault(1);

    /* Init xmlsec library */
    if(xmlSecInit() < 0) {
        fprintf(stderr, "Error: xmlsec initialization failed.\n");
        return(-1);
    }

    /* Load default crypto engine if we are supporting dynamic
     * loading for xmlsec-crypto libraries. Use the crypto library
     * name ("openssl", "nss", etc.) to load corresponding 
     * xmlsec-crypto library.
     */
#ifdef XMLSEC_CRYPTO_DYNAMIC_LOADING
    if(xmlSecCryptoDLLoadLibrary(NULL) < 0) {
        fprintf(stderr, "Error: unable to load default xmlsec-crypto library. Make sure\n"
                        "that you have it installed and check shared libraries path\n"
                        "(LD_LIBRARY_PATH and/or LTDL_LIBRARY_PATH) environment variables.\n");
        return(-1);     
    }
#endif /* XMLSEC_CRYPTO_DYNAMIC_LOADING */

    /* Init crypto library */
    if(xmlSecCryptoAppInit(NULL) < 0) {
        fprintf(stderr, "Error: crypto initialization failed.\n");
        return(-1);
    }

    /* Init xmlsec-crypto library */
    if(xmlSecCryptoInit() < 0) {
        fprintf(stderr, "Error: xmlsec-crypto initialization failed.\n");
        return(-1);
    }

    if(sign_file(filename, "/ca/document-ca/private/document-ca.key", outfile) < 0) {
        return(-1);
    }    
    
    /* Shutdown library */
    xmlSecCryptoShutdown();
    xmlSecCryptoAppShutdown();
    xmlSecShutdown();

    xmlCleanupParser();
    return 0;
}