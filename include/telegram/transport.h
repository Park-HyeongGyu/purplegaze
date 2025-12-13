#ifndef TRANSPORT_H
#define TRANSPORT_H

typedef enum {
    TG_HTTP_OK = 0,

    TG_HTTP_ERR_INIT,        // fail to initialize curl
    TG_HTTP_ERR_INVALID_ARG, // url / payload NULL
    TG_HTTP_ERR_CURL,        // fail curl_easy_perform 
    TG_HTTP_ERR_TIMEOUT,     // timeout
    TG_HTTP_ERR_SSL,         // TLS / certificaiton error
    TG_HTTP_ERR_HTTP_RESPONSE_STATUS, // HTTP response status != 2xx
    TG_HTTP_ERR_INTERNAL
} tg_http_result_t;

tg_http_result_t request_http(const char *url, const char *paylord);

#endif /* TRANSPORT_H */
