GSOAP_ROOT=/usr/local/gSOAP
WSNAME0=soap
WSNAME=SmsWBS
CC=g++ -g -DWITH_NONAMESPACES
INCLUDE=-I $(GSOAP_ROOT)/include
SERVER_OBJS=$(WSNAME0)C.o $(WSNAME0)Server.o stdsoap2.o
ALL_OBJS=${WSNAME}server.o $(WSNAME0)C.o $(WSNAME0)Server.o 


all:server

${WSNAME}.wsdl:${WSNAME}.h
	$(GSOAP_ROOT)/bin/soapcpp2 -c -S $(GSOAP_ROOT)/share/gsoap/import ${WSNAME}.h

stdsoap2.o:$(GSOAP_ROOT)/src/stdsoap2.c
	$(CC) -c $? $(INCLUDE)

$(ALL_OBJS):%.o:%.c
	$(CC) -c $? $(INCLUDE)

server:Makefile ${WSNAME}.wsdl ${WSNAME}server.o $(SERVER_OBJS)
	$(CC) ${WSNAME}server.o $(SERVER_OBJS) -o ${WSNAME}server


clean:
	rm -f *.o *.xml *.a *.wsdl *.nsmap $(WSNAME0)H.h $(WSNAME0)C.c $(WSNAME0)Server.c $(WSNAME0)Stub.* $(WSNAME)$(WSNAME)Proxy.* $(WSNAME)$(WSNAME)Object.* $(WSNAME0)ServerLib.c $(WSNAME)server ns.xsd 
