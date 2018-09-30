#pragma once

#include <memory>

namespace fs {

   enum SeekMode {
      SeekSet = 0,
      SeekCur = 1,
      SeekEnd = 2
   };

   class FileImpl;
   typedef std::shared_ptr<FileImpl> FileImplPtr;

   class File {
   public:

      File( FileImplPtr p = FileImplPtr()) :
         _p( p )
      {}

      operator bool() const;
      size_t read( uint8_t * buf, size_t size );
      size_t size() const;
      void close();

   protected:

      FileImplPtr _p;
   };

   class FS {
   public:

       bool begin();
       File open( const char * path, const char * mode );
   };
}

#ifndef FS_NO_GLOBALS
using fs::FS;
using fs::File;
using fs::SeekMode;
using fs::SeekSet;
using fs::SeekCur;
using fs::SeekEnd;
#endif

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_SPIFFS)
extern fs::FS SPIFFS;
#endif
