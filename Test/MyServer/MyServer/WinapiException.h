#pragma once
#include"stdafx.h"
//#include"Header.h"
//
//
//namespace Example
//{
//
//	class WinapiException : public std::exception
//	{
//
//	protected:
//
//		mutable std::wstring wsText;
//		mutable std::string sText;
//		DWORD dwCode;
//
//	public:
//
//		//using std::exception::exception;
//
//		WinapiException(const std::wstring & wsText = std::wstring(),
//			DWORD dwCode = 0) :  wsText(wsText), dwCode(dwCode) { }
//
//		const char * WinapiException::what(void)const override;
//
//		virtual std::wstring What(void) const;
//
//		DWORD WinapiException::Code(void) const throw()
//		{
//
//			return this->dwCode;
//
//		}
//
//		virtual~WinapiException(){}
//	};
//
//	class ConvertException : public WinapiException
//	{
//
//	public:
//		ConvertException(std::wstring & str, DWORD dw) : WinapiException(str, dw){}
//	
//	};
//
//
//
//	class FileException : public WinapiException
//	{
//
//	public:
//		FileException(std::wstring & str, DWORD dw) : WinapiException(str, dw){}
//		
//		virtual~FileException(){}
//	};
//
//	class NetException
//		: public WinapiException
//	{
//
//	public:
//		NetException(std::wstring & str, DWORD dw) : WinapiException(str, dw){}
//		
//		virtual~NetException(){}
//	};
//
//
//
//
//}