#include <stdio.h>
#include <stdlib.h>
#include "soapH.h"
#include "SmsWBS.nsmap"

int http_get(struct soap *soap);

int main(int argc, char **argv)
{
        int m, s;               /* master and slave sockets */
        struct soap SmsWBS_soap;

        soap_init(&SmsWBS_soap);

	 SmsWBS_soap.fget = http_get;

        soap_set_namespaces(&SmsWBS_soap, namespaces);

        if (argc < 2)
        {
                //printf("usage: %s <server_port> \n", argv[0]);
                //exit(1);
	//CGI 风格服务请求，单线程
          soap_serve(&SmsWBS_soap);
          //清除序列化的类的实例
         soap_destroy(&SmsWBS_soap);
         //清除序列化的数据
        soap_end(&SmsWBS_soap); 
        }
        else
        {
                m = soap_bind(&SmsWBS_soap, NULL, atoi(argv[1]), 100);
                if (m < 0)
                {
                        soap_print_fault(&SmsWBS_soap, stderr);
                        exit(-1);
                }

                fprintf(stderr, "Socket connection successful: master socket = %d\n", m);

                for (;;)
                {
                        s = soap_accept(&SmsWBS_soap);

                        if (s < 0)
                        {
                                soap_print_fault(&SmsWBS_soap, stderr);
                                exit(-1);
                        }

                        fprintf(stderr, "Socket connection successful: slave socket = %d\n", s);
                        soap_serve(&SmsWBS_soap);
			fprintf(stderr, "soap_serve() 调用结束\n");
                        soap_end(&SmsWBS_soap);
			fprintf(stderr, "soap_end() 调用结束\n");
                }

       }

        return 0;
}

int http_get(struct soap *soap) 
{ 
        FILE*fd = NULL;

	char *s = strchr( soap->path, '?' );
         if( !s || strcmp( s, "?wsdl" ) ){
             return SOAP_GET_METHOD;
         }

        fd = fopen("SmsWBS.wsdl", "rb"); //open WSDL file to copy
        if (!fd)
        {
                return 404; //return HTTP not found error
        }
        soap->http_content = "text/xml";  //HTTP header with text /xml content
        soap_response(soap,SOAP_FILE);
        for(;;)
        {
                size_t r = fread(soap->tmpbuf,1, sizeof(soap->tmpbuf), fd);
                if (!r)
                {
                        break;
                }
                if (soap_send_raw(soap, soap->tmpbuf, r))
                {
                        break; //cannot send, but little we can do about that
                }
        }
        fclose(fd);
        soap_end_send(soap);
        return SOAP_OK; 
}

int ns__add(struct soap *add_soap, int num1, int num2, int *sum)
{
        *sum = num1 + num2;
        return 0;
}
