#include "WebClient.h"



WebClient::WebClient()
{
}


WebClient::~WebClient()
{
}

/*
Returns NULL on failure.
*/
CURL *WebClient::init()
{
	curl = curl_easy_init();
	if (curl)
	{
		curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);

		curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
		curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
	}
	else
	{
		return NULL;
	}

	return curl;
}

/*
Reset header and response strings, then use libcurl to read data into them
*/
CURLcode WebClient::perform()
{
	header.clear();
	response.clear();

	CURLcode res = curl_easy_perform(curl);

	return res;
}

/*
Searches for a header field type and fills info into buffer
*/
bool WebClient::getHeaderInfo(std::string fieldType, std::string *info)
{
	unsigned int posA = 0;
	unsigned int posB = 0;
	fieldType = "\r\n" + fieldType + ": ";

	posA = header.find(fieldType, 13); //Shortest start size: (GET/ PUT) / HTTP/1.1 = 14 chars
	if (posA != std::string::npos)
	{
		posA += fieldType.length();
		posB = header.find("\r\n", posA);
		if (posB != std::string::npos)
		{
			*info = header.substr(posA, posB - posA);
			return true;
		}
	}

	return false;
}

/*
Uses libcurl to identify if the HTTP request was successful
*/
int WebClient::checkSucc()
{
	long code;

	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

	if (code >= 200 && code < 300)
	{
		return WEB_SUCCESS;
	}
	else
	{
		if (code >= 100 && code < 200)
		{
			return WEB_INFORMATIONAL;
		}
		else if (code >= 300 && code < 400)
		{
			return WEB_REDIRECTION;
		}
		else if (code >= 400 && code < 500)
		{
			return WEB_CLIENT_ERROR;
		}
		else if (code >= 500 && code < 600)
		{
			return WEB_SERVER_ERROR;
		}
		else
		{
			return WEB_UNKNOWN_CODE;
		}
	}
}

void WebClient::cleanup()
{
	curl_easy_cleanup(curl);
}

CURL *WebClient::getCurl()
{
	return curl;
}

void WebClient::setCurl(CURL *curl)
{
	this->curl = curl;
}

void WebClient::removeAndSetCurl(CURL *curl)
{
	cleanup();
	setCurl(curl);
}

std::string WebClient::getHeader()
{
	return header;
}

void WebClient::setHeader()
{
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
}

std::string WebClient::getResponse()
{
	return response;
}

void WebClient::setResponse()
{
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
}

unsigned int writeFunction(void *ptr, unsigned int size, unsigned int nmemb, std::string *data)
{
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}