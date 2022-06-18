#ifndef __DNC_UTF8_H__ 
#define __DNC_UTF8_H__ 
//############################################################################## 
//The dnc Library 
//Copyright (c) 2005 Dreamsoft 赵纯华 
//Last update: 2005-1-10 
//UTF8转换函数 
//############################################################################## 
// 
#ifndef __DNC_DEFINE_H__ 
#include "define.h" 
#endif 
#ifndef __DNC_ARRAY_H__ 
#include "array.h" 
#endif 
#include <string> 
 
 
namespace dnc{ 
	DNC_DECLARE size_t ANSIToUTF8(castr srcData,size_t srcCount,ustr destData,size_t destCount); 
	DNC_DECLARE size_t UTF8ToANSI(custr srcData,size_t srcCount, astr destData,size_t destCount); 
	DNC_DECLARE size_t ANSIToUNICODE(castr srcData,size_t srcCount, wstr destData,size_t destCount) ; 
	DNC_DECLARE size_t UNICODEToANSI(cwstr srcData,size_t srcCount,astr destData,size_t destCount) ; 
	DNC_DECLARE size_t UNICODEToUTF8(cwstr srcData,size_t srcCount,ustr destData,size_t destCount) ; 
	DNC_DECLARE size_t UTF8ToUNICODE(custr srcData,size_t srcCount, wstr destData,size_t destCount) ; 
 
	inline size_t ANSIToUTF8(const Array<achar> &src,Array<uchar> &dest){ 
		return ANSIToUTF8(src.data(),src.size(),dest.data(),dest.size()); 
	} 
	inline size_t UTF8ToANSI(const Array<uchar> &src,Array<achar> &dest){ 
		return UTF8ToANSI(src.data(),src.size(),dest.data(),dest.size()); 
	} 
	inline size_t UNICODEToUTF8(const Array<wchar> &src,Array<uchar> &dest){ 
		return UNICODEToUTF8(src.data(),src.size(),dest.data(),dest.size()); 
	} 
	inline size_t UTF8ToUNICODE(const Array<uchar> &src,Array<wchar> &dest){ 
		return UTF8ToUNICODE(src.data(),src.size(),dest.data(),dest.size()); 
	} 
 
    DNC_DECLARE std::string UTF8ToANSI(const std::string &srcData) dnc_reg(); 
    DNC_DECLARE std::string ANSIToUTF8(const std::string &srcData) dnc_reg(); 
 
	//把一个xchar转换成UTF-8编码 
	//parameters: 
	//	ch   任意的字符 
	//	utf8 不小于6字节的缓冲区,存储ch转换的结果 
	//return: 
	//	ch 转换成utf8后占用的字节数 
	DNC_DECLARE unsigned int XCharToUTF8(xchar ch,astr utf8); 
	//把一个utf8字符转换成任意字符xchar 
	//parameters: 
	//	utf8  存储一个字符的UTF-8格式 
	//	ch	  用于存储utf8转换的结果 
	//return: 
	//	utf8 表示的一个字符占用的字节数 
	DNC_DECLARE unsigned int UTF8ToXChar(custr utf8,xchar &ch); 
	 
 
	//UTF-8字符串操作函数 
	//默认情况下此函数以标准库的strcmp函数实现，我们也可以设置系统参数 
	//使得strcmp以更为人性的方式比较字符串。比如以逻辑的字符值或者是按照 
	//汉字拼音比较。 
	DNC_DECLARE int	 utf8_strcmp(castr str1,castr str2,unsigned int count= (unsigned int)-1); 
	//返回值 
	//size    str的逻辑长度,字符数 
	//rawSize str的物理长度,以0结尾的缓冲区长度 
	DNC_DECLARE void utf8_strlen(castr str,unsigned int &size,unsigned int &rawSize,unsigned int count= (unsigned int)-1); 
	DNC_DECLARE xchar utf8_value(custr str); 
 
	extern DNC_DECLARE cuchar gUTFBytes[256]; 
	extern DNC_DECLARE cuchar gFirstByteMark[7]; 
	extern DNC_DECLARE const unsigned long gUTFOffsets[6]; 
 
 
	///////////////////////////////////////// 
	//utf8字符跌代器 
	class utf8_const_iterator{ 
	public: 
		typedef xchar		value_type; 
		typedef value_type	reference; 
		typedef utf8_const_iterator MyType; 
	public: 
		utf8_const_iterator(castr it):m_it((ustr)it){ 
            //如果it刚好在一个utf8字符的中间，则向前边推算出这个字符 
			for(;*m_it >= 0x80 && *m_it < 0xE0 && (ustr)it - m_it < 6;--m_it); 
		} 
		reference operator*() const{ 
			return *m_it <= 127 ? *m_it : utf8_value(m_it); 
		} 
		operator castr () const{ 
			return (castr)m_it; 
		} 
        int get_charSize() const{ 
            return gUTFBytes[*m_it]+1; 
        } 
		long operator - (const utf8_const_iterator &other) const{ 
			//long size; 
			//for(utf8_const_iterator it=*this;it!=other;it++) 
			//return (size_t)(m_it-it.m_it); 
			return 0; 
		} 
 
        MyType operator + (int offset) const{ 
            if(offset < 0) 
                return operator-(-offset); 
 
            const uchar *str = m_it; 
            for(int i=0;i<offset;i++) 
                str += gUTFBytes[*str]+1; 
            return MyType((castr)str); 
        } 
        MyType operator - (int offset) const{ 
            if(offset < 0) 
                return operator+(-offset); 
 
            const uchar *str = m_it; 
            for(int i=0;i<offset;i++) 
                for(--str;*str >= 0x80 && *str < 0xE0;--str); 
            return MyType((castr)str); 
        } 
		MyType& operator ++(){ 
			m_it += gUTFBytes[*m_it]+1; 
			return *this; 
		} 
		MyType operator ++(int){ 
			MyType tmp = *this; 
			m_it += gUTFBytes[*m_it]+1; 
			return tmp; 
		} 
		MyType& operator --(){ 
			//如果是在1000 0000(0x80)和1100 0000(0xE0)之间的值就忽略掉 
			for(--m_it;*m_it >= 0x80 && *m_it < 0xE0;--m_it); 
			return *this; 
		} 
		MyType operator --(int){ 
			MyType tmp = *this; 
			for(--m_it;*m_it >= 0x80 && *m_it < 0xE0;--m_it); 
			return tmp; 
		} 
		bool operator == (const MyType &right){ 
			return m_it==right.m_it; 
		} 
		bool operator != (const MyType &right){ 
			return !(*this == right); 
		} 
		bool operator<(const MyType &right) const{ 
			return m_it<right.m_it; 
		} 
		bool operator>(const MyType& right) const{ 
			return (right < *this); 
		} 
		bool operator<=(const MyType& right) const{ 
			return (!(right < *this)); 
		} 
		bool operator>=(const MyType& right) const{ 
			return (!(*this < right)); 
		} 
	protected: 
		ustr m_it; 
	}; 
	class utf8_iterator : public utf8_const_iterator{ 
	public: 
		typedef utf8_iterator MyType; 
	public: 
		utf8_iterator(castr it):utf8_const_iterator(it){ 
		} 
		MyType& operator ++(){ 
			m_it += gUTFBytes[*m_it]+1; 
			return *this; 
		} 
		MyType operator ++(int){ 
			MyType temp = *this; 
			m_it += gUTFBytes[*m_it]+1; 
			return temp; 
		} 
		MyType& operator --(){ 
			for(--m_it;*m_it >= 0x80 && *m_it < 0xE0;--m_it); 
			return *this; 
		} 
		MyType operator --(int){ 
			MyType tmp = *this; 
			for(--m_it;*m_it >= 0x80 && *m_it < 0xE0;--m_it); 
			return tmp; 
		} 
	}; 
 
 
	//////////////////////////////////// 
	//反向跌代器 
	template<class Base> 
	class utf8_reverse_bidirectional_iterator{ 
	public: 
		typedef typename Base::value_type    value_type; 
		typedef typename Base::reference 	reference; 
		typedef utf8_reverse_bidirectional_iterator MyType; 
	public: 
		utf8_reverse_bidirectional_iterator(const Base &it):m_it(it){} 
		Base base() const{ 
			return (m_it); 
		} 
 
		reference operator*() const{ 
			return *m_it; 
		} 
		MyType& operator ++(){ 
			--m_it; 
			return *this; 
		} 
		MyType operator ++(int){ 
			MyType tmp = *this; 
			--m_it; 
			return tmp; 
		} 
		MyType& operator --(){ 
			++m_it; 
			return *this; 
		} 
		MyType operator --(int){ 
			MyType tmp = *this; 
			++m_it; 
			return tmp; 
		} 
		bool operator == (const MyType &right){ 
			return m_it==right.m_it; 
		} 
		bool operator != (const MyType &right){ 
			return !(*this == right); 
		} 
		bool operator<(const MyType &right) const{ 
			return m_it<right.m_it; 
		} 
		bool operator>(const MyType& right) const{ 
			return (right < *this); 
		} 
		bool operator<=(const MyType& right) const{ 
			return (!(right < *this)); 
		} 
		bool operator>=(const MyType& right) const{ 
			return (!(*this < right)); 
		} 
	private: 
		Base m_it; 
	}; 
	typedef utf8_reverse_bidirectional_iterator<utf8_iterator> utf8_reverse_iterator; 
	typedef utf8_reverse_bidirectional_iterator<utf8_const_iterator> utf8_const_reverse_iterator; 
} 
 
#endif //__DNC_UTF8_H__ 

 