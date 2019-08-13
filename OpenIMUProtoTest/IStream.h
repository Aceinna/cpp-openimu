#ifndef _ICUSTOM_STREAM_H
#define _ICUSTOM_STREAM_H

#define INFINITE_TIMEOUT 0xffffffff

class ICustomStream{

	virtual int read(char* buf, int len, int timeout = INFINITE_TIMEOUT)  = 0;
	virtual int write(char* buf, int len, int timeout = INFINITE_TIMEOUT) = 0;
	virtual int readByte(char* buf, int len, int timeout = INFINITE_TIMEOUT){
		return read(buf, 1, timeout);
	};
	virtual int writeByte(char* buf, int len, int timeout = INFINITE_TIMEOUT){
		return write(buf, 1, timeout);
	};

	virtual int close();
};


#endif
