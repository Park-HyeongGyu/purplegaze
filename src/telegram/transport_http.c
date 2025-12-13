#include <curl/curl.h>
#include "telegram/transport_http.h"

tg_http_err_t send_http(const char *url, const char *paylord);

/*
 * send_http
 * Sends an HTTP POST request to the given URL with the given paylord.
 * It is only made for transportation. It does not parse paylord or url;
 * therefore requires authetic url and paylord.
 * 
 * Parameters:
 *  url     - target HTTPS url
 *  paylord - Request body (e.g. JSON)
 * 
 * Returns:
 *  tg_http_err_t indicates whether transportation successed or failed.
*/
tg_http_err_t send_http(const char *url, const char *paylord){
    /* Valudate input auguments */
    if (url == NULL || paylord == NULL){
        return TG_HTTP_ERR_INVALID_ARG;
    }

    /* Create a new libcurl handle */
    CURL *curl = curl_easy_init();
    if (curl == NULL){
        return TG_HTTP_ERR_INIT;
    }

    /* Set target URL  */
    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Use HTTP POST */
    curl_easy_setopt(curl, CURLOPT_POST, 1L);

    /* Set POST request body*/
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, paylord);

    /*
     * Enbable TLS certificate verification.
     * Even though those are enabled by defualt,
     * I will set them explicitly for clarity and safety
     * since it is my first time using curl.
     */
    curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_PROXY_SSL_VERIFYHOST, 2L);

    /* Set connection and total request imeouts (in seconds)*/
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    /* Perform the HTTPS request (blocking call) */
    CURLcode err = curl_easy_perform(curl);
    if (err != CURLE_OK){
        curl_easy_cleanup(curl);

        if (err == CURLE_OPERATION_TIMEDOUT){
            return TG_HTTP_ERR_TIMEOUT;
        }
        if (err == CURLE_SSL_CONNECT_ERROR){
            return TG_HTTP_ERR_SSL;
        }

        return TG_HTTP_ERR_CURL;
    }

    /* Get HTTP response status code */
    long http_response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);

    /* Clean up the handle that I used */
    curl_easy_cleanup(curl);

    /* Handle HTTP response status code */
    if (http_response_code < 200 || http_response_code >= 300){
        return TG_HTTP_ERR_HTTP_RESPONSE_STATUS;
    }

    return TG_HTTP_OK;
}
