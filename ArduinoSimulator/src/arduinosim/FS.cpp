#include "FS.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

namespace fs {
   class FileImpl {
   public:

      FileImpl( int fd_ ) :
         fd( fd_ )
      {}

      int fd;
   };
}

using namespace fs;

File::operator bool() const {
   return _p && _p->fd;
}

size_t File::read( uint8_t * buf, size_t size ) {
   return ::read( _p->fd, buf, size );
}

size_t File::size() const {
   struct stat s;
   ::fstat( _p->fd, &s );
   return s.st_size;
}
void File::close() {
   ::close( _p->fd );
}

bool FS::begin() {
   return true;
}

File FS::open( const char * path, const char * ) {
   return File( std::shared_ptr<FileImpl>( new FileImpl( ::open( path, O_RDONLY ))));
}

fs::FS SPIFFS;
