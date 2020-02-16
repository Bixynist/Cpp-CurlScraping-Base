#pragma once

//#define CURL_STATICLIB

#define WEB_UNKNOWN_CODE 0
#define WEB_INFORMATIONAL 1
#define WEB_SUCCESS 2
#define WEB_REDIRECTION 3
#define WEB_CLIENT_ERROR 4
#define WEB_SERVER_ERROR 5

#include <iostream>
#include <vector>
#include <unordered_map>

#include <curl/curl.h>

unsigned int writeFunction(void *ptr, unsigned int size, unsigned int nmemb, std::string *data);

/*
Reusable webscraper base
*/
class WebClient
{
public:

public:
	WebClient();
	~WebClient();

	virtual CURL *init();
	virtual CURLcode perform();
	bool getHeaderInfo(std::string fieldType, std::string *info);
	int checkSucc();
	virtual void cleanup();

protected:
	CURL *curl;
	std::string response, header;

protected:
	
private:

private:

public:
	CURL *getCurl();
	void setCurl(CURL *curl);
	void removeAndSetCurl(CURL *curl);

	std::string getHeader();
	void setHeader();

	std::string getResponse();
	void setResponse();
};