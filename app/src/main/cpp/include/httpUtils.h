#pragma once
#include <curl/curl.h>
#include <openssl/ossl_typ.h>
#include <openssl/bio.h>
#include <openssl/ssl.h>
#include "certificate.h"

using namespace std;

bool enabled = false; //true -- https  false -- http

struct MemoryObject {
    char *data;
    size_t size;
};

static size_t write_callback(void *content, size_t size, size_t nmemb, void *out) {
    if (1 != size) {
        return 0;
    }
    MemoryObject *obj = (MemoryObject *)out;
    size_t byte_size = size * nmemb;
    size_t old_size = obj->size;
    if (0 == obj->size) {
        obj->size = byte_size;
        obj->data = (char *)malloc(obj->size);
    } else {
        obj->size += byte_size;
        obj->data = (char *)realloc(obj->data, obj->size);
    }
    memcpy(obj->data + old_size, content, nmemb);
    return byte_size;
}

static CURLcode sslctx_function(CURL *curl, void *sslctx, void *parm) {
    CURLcode rv = CURLE_ABORTED_BY_CALLBACK;
    BIO *cbio = BIO_new_mem_buf(pem_from_haxx_se, sizeof(pem_from_haxx_se));
    X509_STORE *cts = SSL_CTX_get_cert_store((SSL_CTX *)sslctx);
    int i;
    STACK_OF(X509_INFO) * inf;
    (void)curl;
    (void)parm;
    if (!cts || !cbio) {
        return rv;
    }
    inf = PEM_X509_INFO_read_bio(cbio, NULL, NULL, NULL);
    if (!inf) {
        BIO_free(cbio);
        return rv;
    }
    for (i = 0; i < sk_X509_INFO_num(inf); i++) {
        X509_INFO *itmp = sk_X509_INFO_value(inf, i);
        if (itmp->x509) {
            X509_STORE_add_cert(cts, itmp->x509);
        }
        if (itmp->crl) {
            X509_STORE_add_crl(cts, itmp->crl);
        }
    }
    sk_X509_INFO_pop_free(inf, X509_INFO_free);
    BIO_free(cbio);
    rv = CURLE_OK;
    return rv;
}


string post(string url, string param,bool enable) {
    // std::cout << "POST: " << url << endl;
    // std::cout << "Data: " << param << std::endl;
    MemoryObject response_header;
    response_header.data = nullptr;
    response_header.size = 0;
    MemoryObject response_body;
    response_body.data = nullptr;
    response_body.size = 0;
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, param.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, param.size());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, *write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_body);
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, *write_callback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&response_header);
    curl_easy_setopt(curl, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
    if (enable) {
        curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
        curl_easy_setopt(curl, CURLOPT_CAINFO, NULL);
        curl_easy_setopt(curl, CURLOPT_CAPATH, NULL);
        curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, *sslctx_function);
        static std::string https("https://");
        if (url.substr(0, 8) == https) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            // cout << url << endl;
        } else {
            std::string url_with_https(https + url);
            curl_easy_setopt(curl, CURLOPT_URL, url_with_https.c_str());
            // cout << url_with_https << endl;
        }
    } else {
        static std::string http("http://");
        if (url.substr(0, 7) == http) {
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        } else {
            std::string url_with_http(http + url);
            curl_easy_setopt(curl, CURLOPT_URL, url_with_http.c_str());
        }
    }
    std::string result;
    CURLcode ret = curl_easy_perform(curl);
    if (ret != CURLE_OK) {
        int a = 0;
        //      std::cout << "error:" << ret << std::endl;
    } else {
        result = std::string(response_body.data, response_body.size);
    }
    // std::cout << result << std::endl;
    curl_easy_cleanup(curl);
    return result;
}