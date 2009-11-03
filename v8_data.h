#ifndef __v8_data_h__
#define __v8_data_h__

#include "v8.h"
#include "stdint.h"
#include <stdio.h>
#include <string>


template<class T, class R> class V8HandleSource {

  T dest;

 public:

  V8HandleSource() {}
  ~V8HandleSource() {}

  R push(v8::Handle<v8::Value>& value, const char* name = 0) {

    if (value->IsUndefined()) {
      return dest.pushNull(name);
    }

    if(value->IsNull()) {
      return dest.pushNull(name);
    }
    
    if(value->IsTrue()) {
      return dest.pushBool(true, name);
    }

    if(value->IsFalse()) {
      return dest.pushBool(false, name);
    }

    if(value->IsString()) {
      v8::Local<v8::String> str = value->ToString();
      char buffer[1024];
      int strlen = str->Length();
      std::string output(strlen, 0);
      for (int total = 0; total < strlen;) {
        int written = str->WriteAscii(buffer, total, 1024);
        output.replace(total, written, buffer);
        total += written;
      }
      return dest.pushString(output, name);
    }

    if(value->IsInt32()) {
      return dest.pushInt(value->Int32Value(), name);
    }

    if(value->IsNumber()) {
      return dest.pushDouble(value->NumberValue(), name);
    }
    
    if (value->IsObject()) {
      v8::Local<v8::Object> object(v8::Object::Cast(*value));
      return dest.pushObject(object, name);
    }
    
    return dest.pushNull(name);
  }

};

#endif