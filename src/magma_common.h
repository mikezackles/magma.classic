
/**
 * @file /magma/magma_common.h
 *
 * @brief The global include file.
 *
 * $Author$
 * $Date$
 * $Revision$
 *
 */

#ifndef MAGMA_COMMON_H
#define MAGMA_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>
#include <limits.h>
#include <signal.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <execinfo.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/utsname.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <netdb.h>
#include <resolv.h>
#include <regex.h>
#include <ftw.h>
#include <search.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <math.h>

// GNU C Library
#include <gnu/libc-version.h>

// External dependencies
#include "magma_external.h"

/**
 * The type definitions used by Magma that are not defined by the system headers.
 * The bool type requires the inclusion of stdbool.h and the use of the C99.
 */
typedef char chr_t;
typedef bool bool_t;
typedef int32_t int_t;
typedef uint32_t uint_t;
typedef unsigned char uchr_t;
typedef unsigned char byte_t;

// RFC 2181 specifies a maximum legal length of 253 characters for a dotted domain name. Individual levels of the overall name
// may contain a maximum of 63 characters.
#define MAGMA_HOSTNAME_MAX _POSIX_HOST_NAME_MAX

// The maximum number of characters in a file path... currently 4096.
#define MAGMA_FILEPATH_MAX PATH_MAX

// The maximum number of characters in a file name... currently 255.
#define MAGMA_FILENAME_MAX NAME_MAX

// The amount of memory allocated by default to hold the stack for spawned threads.
#define MAGMA_THREAD_STACK_SIZE 1048576

// The size of the thread local buffer.
#define MAGMA_THREAD_BUFFER_SIZE 1024

// The maximum number of worker threads allowed, even if the system limit is higher.
#define MAGMA_WORKER_THREAD_LIMIT 16384

// The amount of data used to seed the random number generator.
#define MAGMA_CRYPTOGRAPHY_SEED_SIZE 64

// The default resource paths.
#define MAGMA_LOGS "logs/"
#define MAGMA_RESOURCE_FONTS "resources/fonts/"
#define MAGMA_RESOURCE_PAGES "resources/pages/"
#define MAGMA_RESOURCE_VIRUS "resources/virus/"
#define MAGMA_RESOURCE_LOCATION "resources/location/"
#define MAGMA_RESOURCE_TEMPLATES "resources/templates/"

// The default location database caching policy.
#define MAGMA_LOCATION_CACHE CONSTANT("disable")

// The maximum number of server instances.
#define MAGMA_CACHE_INSTANCES 8

// The default caching server retry interval.
#define MAGMA_CACHE_SERVER_RETRY 600

// The default caching server connection timeout.
#define MAGMA_CACHE_SOCKET_TIMEOUT 10

// The maximum number of server instances.
#define MAGMA_BLACKLIST_INSTANCES 6

// The maximum number of relay instances.
#define MAGMA_RELAY_INSTANCES 8

// The maximum number of server instances.
#define MAGMA_SERVER_INSTANCES 32

// The default size of connection buffer. Can be changed via the config.
#define MAGMA_CONNECTION_BUFFER_SIZE 8192

// The maximum size of the HELO/EHLO string.
// RFC 2821, section 4.5.3.1 dictates a max length of 255 characters for a domain
#define MAGMA_SMTP_MAX_HELO_SIZE MAGMA_HOSTNAME_MAX

// RFC 2821, section 4.5.3.1 dictates a max length of 256 characters for an address.
#define MAGMA_SMTP_MAX_ADDRESS_SIZE 256

// The default limit on recipients.
#define MAGMA_SMTP_RECIPIENT_LIMIT 256

#define MAGMA_SMTP_RELAY_LIMIT 256

// The default maximum line length for SMTP messages.
// RFC 3676 / 4.2 ... Lines SHOULD be 78 characters or shorter, including any trailing
// white space and also including any space added as part of stuffing...
#define MAGMA_SMTP_LINE_WRAP_LENGTH 80

// The maximum size of a message accepted via SMTP.
#define MAGMA_SMTP_MAX_MESSAGE_SIZE 1073741824

#include "core/core.h"

typedef enum {
	TCP_PORT = 1,
	SSL_PORT
} M_PORT;

typedef enum {
	MOLTEN = 1,
	HTTP,
	POP,
	IMAP,
	SMTP,
	SUBMISSION
} M_PROTOCOL;

typedef struct {
	struct {
		SSL_CTX *context;
		char *certificate;
	} ssl;
	struct {
		int sockd;
		bool_t ipv6;
		uint32_t port;
		uint32_t timeout;
		uint32_t listen_queue;
		M_PORT type;
	} network;
	struct {
		uint32_t delay;
		uint32_t cutoff;
	} violations;
	bool_t enabled;
	stringer_t *name, *domain;
	M_PROTOCOL protocol;
} server_t;

typedef struct {
	bool_t secure; /* Use SSL. */
	bool_t premium; /* Reserve for premium users. */
	chr_t *name; /* The relay name. */
	uint32_t port; /* The relay port. */
} relay_t;

typedef struct {
	char *name; /* The cache name. */
	uint32_t port; /* The cache port. */
	uint32_t weight; /* The key space weight. */
} cache_t;

// The magma config type
typedef struct {

	struct {
		bool_t output_config; /* Dump the configuration to the log file. */
		bool_t output_resource_limits; /* Should attempts to increase system limits trigger an error. */

		// LOW: Filenames are limited to 255 characters, but file paths can be up to 4096. As such we should probably be storing this using a block of memory off the heap.
		chr_t file[MAGMA_FILEPATH_MAX + 1]; /* Path to the magmad.config file. */
	} config;

	struct {
		stringer_t *contact; /* The general purpose contact email address. */
		stringer_t *abuse; /* The contact email address for abuse complaints. */
	} admin;

	struct {
		uint64_t number; /* The unique numeric identifier for this host. */
		chr_t name[MAGMA_HOSTNAME_MAX + 1]; /* The hostname. Size can be MAGMA_HOSTNAME_MAX (64) or MAGMA_HOSTNAME_MAX (255). Make sure the gethostname() calls match. */
	} host;

	struct {
		chr_t *file; /* Path to the magma.open.so library. */
		bool_t unload; /* Unload the magma.open.so symbols at exit. */
	} library;

	struct {
		bool_t daemonize; /* Spawn a daemon process and release the console session. */
		char * root_directory; /* Change the root path to the provided value. */
		char * impersonate_user; /* Change the effective user account of the process to the user provided. */
		bool_t increase_resource_limits; /* Attempt to increase system limits. */
		uint32_t thread_stack_size; /* How much memory should be allocated for thread stacks? */
		uint32_t worker_threads; /* How many worker threads should we spawn? */
		uint32_t network_buffer; /* The size of the network buffer? */

		bool_t enable_core_dumps; /* Should fatal errors leave behind a core dump. */
		uint64_t core_dump_size_limit; /* If core dumps are enabled, what size should they be limited too. */

		stringer_t *domain; /* The default domain name used in new user email addresses and for unqualified login names. */
	} system;

	struct {
		struct {
			bool_t enable; /* Should the secure memory sub-system be enabled. */
			uint64_t length; /* The size of the secure memory pool. The pool must fit within any memory locking limits. */
		} memory;

		stringer_t *salt; /* The string added to hash operations to improve security. */
		stringer_t *links; /* The string used to encrypt links that reflect back to the daemon. */
		stringer_t *sessions; /* The string used to encrypt session tokens. */
	} secure;

	struct {
		bool_t file; /* Send log messages to a file. */
		chr_t *path; /* If log files are enabled, this will control whether the logs are stored. */
	} output;

	struct {

		bool_t imap; /* Output IMAP request details. */
		bool_t http; /* Output HTTP request details. */
		bool_t content; /* Output the web resource files loaded. */

		bool_t file; /* Output the source file that recorded the log entry. */
		bool_t line; /* Output the source line that recorded the log entry. */
		bool_t time; /* Output time that the log entry was recorded. */
		bool_t stack; /* Output the stack that triggered the log entry. */
		bool_t function; /* Output the function that made the log entry. */
	} log;

	struct {
		chr_t *tank; /* The path of the storage tank. */
		stringer_t *active; /* The default storage server used by the legacy mail storage logic. */
		stringer_t *root; /* The root portion of the storage server directory paths. */
	} storage;

	struct {
		uint32_t relay_limit; /* The number of relay hops allowed before assuming a message is stuck in a loop and discarding it. */
		uint32_t recipient_limit; /* The maximum number of recipients for an inbound/outbound message. */
		uint32_t address_length_limit; /* What is the maximum size of a legal email address? */
		uint32_t helo_length_limit; /* How big of a HELO/EHLO parameter will the system allow? */
		uint32_t wrap_line_length; /* When formatting email messages attempt to wrap lines longer than this length. */
		uint64_t message_length_limit; /* How big of a message will the system allow via SMTP? */

		// Store information about the realtime blacklists used to block messages via SMTP.
		struct {
			uint32_t count;
			stringer_t *domain[MAGMA_BLACKLIST_INSTANCES];
		} blacklists;

		stringer_t *bypass_addr; /* Bypass address/subnet string for smtp checks. This value used only by config. */
		inx_t *bypass_subnets; /* Holder for all the address/subnets to be waived through for bypass */
	} smtp;

	struct {
		bool_t close; /* Automatically close HTTP connections after each request? */
		bool_t allow_cross_domain; /* Provide the necessary headers in response to OPTION requests to allow cross domain JSON-RPC requests. */
		chr_t *fonts; /* The web fonts directory. */
		chr_t *pages; /* The static web pages directory. */
		chr_t *templates; /* The web application templates. */
		uint32_t session_timeout; /* Number of seconds before a session cookie expires. */
	} http;

	struct {
		relay_t *host[MAGMA_RELAY_INSTANCES];
		struct {
			uint32_t premium;
			uint32_t standard;
		} count;
		uint32_t timeout;
	} relay;

	struct {
		struct {
			bool_t indent; /* Format the JSON responses before returning them? */
			bool_t safeguard; /* Whether to require HTTPS for access to the portal. */
		} portal;
		struct {
			stringer_t *sender; /* Format the JSON responses before returning them? */
		} contact;
		bool_t statistics; /* Whether or not the statistics page is enabled. */
		bool_t registration; /* Whether or not the new user registration page is enabled. */
		stringer_t *ssl_redirect; /* An ssl hostname and/or port for redirecting secure-only web requests. */
	} web;

	struct {
		bool_t enabled; /* whether or not dkim signing is enabled */
		chr_t *domain;
		chr_t *selector;
		stringer_t *privkey; /* location of the dkim private key at startup (replaced with contents later) */
	} dkim;

	struct {

		struct {
			uint32_t seed_length; /* How much data should be used to seed the random number generator. */
		} cryptography;

		struct {
			chr_t *host; /* The database host name. */
			chr_t *user; /* The database user name. */
			chr_t *password; /* The database password. */
			chr_t *schema; /* The database schema name. */
			uint32_t port; /* The database server port. */
			chr_t *socket_path; /* The database UNIX domain socket path. */

			struct {
				uint32_t timeout; /* The number of seconds to wait for a free database connection. */
				uint32_t connections; /* The number of database connections in the pool. */
			} pool;
		} database;

		struct {
			bool_t available; /* Is ClamAV loaded at runtime. */
			char *signatures; /* The signatures directory. */
		} virus;

		struct {
			chr_t *path; /* The IP location database files. */
			stringer_t *cache; /* The IP location caching strategy. [disable | index | mapped | memory]" */
		} location;

		struct {
			cache_t *host[MAGMA_CACHE_INSTANCES];
			struct {
				uint32_t timeout; /* The number of seconds to wait for a free cache context. */
				uint32_t connections; /* The number of cache client objects to hold in the pool. */
			} pool;
			uint32_t retry; /* How often should dead caching servers be retried. */
			uint32_t timeout; /* The TCP socket send/recv timeout. */
		} cache;

		struct {
			struct {
				uint32_t timeout; /* The number of seconds to wait for a free SPF instance. */
				uint32_t connections; /* The number of SPF instances in the pool. */
			} pool;
		} spf;

	} iface;

	// Global config section
	chr_t * spool; /* The spool directory. */
	int_t page_length; /* The memory page size. This value is used to align memory mapped files to page boundaries. */

	// Global variables section
	uint32_t init; /* How far into the initialization process we've gotten. */
	pthread_rwlock_t lock; /* Used to grab a global read or write lock on the configuration. */
	server_t *servers[MAGMA_SERVER_INSTANCES]; /* An array of server structures. */

} magma_t;

// Global config instance
extern magma_t magma;

extern __thread char threadBuffer[1024];
#define bufptr (char *)&(threadBuffer)
#define buflen sizeof(threadBuffer)

/*
#define HAVE_ABI_CXA_DEMANGLE 1
#define HAVE_ACCESS 1
#define HAVE_AIO_H 1
#define HAVE_ALARM 1
#define HAVE_ALLOCA 1
#define HAVE_ALLOCA_H 1
#define HAVE_ANSIDECL_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_ARPA_NAMESER_H 1
#define HAVE_ASM_TERMBITS_H 1
#define HAVE_ASSERT_H 1
#define HAVE_ATEXIT 1
#define HAVE_ATOI 1
#define HAVE_ATOL 1
#define HAVE_BACKTRACE 1
#define HAVE_BACKTRACE_SYMBOLS 1
#define HAVE_BACKTRACE_SYMBOLS_FD 1
#define HAVE_BOOL 1
#define HAVE_BSEARCH 1
#define HAVE_BSS_START 1
#define HAVE_BZERO 1
#define HAVE_CEILF 1
#define HAVE_CHARSET_armscii8 1
#define HAVE_CHARSET_ascii 1
#define HAVE_CHARSET_big5 1
#define HAVE_CHARSET_cp1250 1
#define HAVE_CHARSET_cp1251 1
#define HAVE_CHARSET_cp1256 1
#define HAVE_CHARSET_cp1257 1
#define HAVE_CHARSET_cp850 1
#define HAVE_CHARSET_cp852 1
#define HAVE_CHARSET_cp866 1
#define HAVE_CHARSET_cp932 1
#define HAVE_CHARSET_dec8 1
#define HAVE_CHARSET_eucjpms 1
#define HAVE_CHARSET_euckr 1
#define HAVE_CHARSET_gb2312 1
#define HAVE_CHARSET_gbk 1
#define HAVE_CHARSET_geostd8 1
#define HAVE_CHARSET_greek 1
#define HAVE_CHARSET_hebrew 1
#define HAVE_CHARSET_hp8 1
#define HAVE_CHARSET_keybcs2 1
#define HAVE_CHARSET_koi8r 1
#define HAVE_CHARSET_koi8u 1
#define HAVE_CHARSET_latin1 1
#define HAVE_CHARSET_latin2 1
#define HAVE_CHARSET_latin5 1
#define HAVE_CHARSET_latin7 1
#define HAVE_CHARSET_macce 1
#define HAVE_CHARSET_macroman 1
#define HAVE_CHARSET_sjis 1
#define HAVE_CHARSET_swe7 1
#define HAVE_CHARSET_tis620 1
#define HAVE_CHARSET_ucs2 1
#define HAVE_CHARSET_ujis 1
#define HAVE_CHARSET_utf8 1
#define HAVE_CHMOD 1
#define HAVE_CHOWN 1
#define HAVE_C_INLINE
#define HAVE_CLOCK_GETTIME 1
#define HAVE_COMPRESS 1
#define HAVE_CRYPT 1
#define HAVE_CRYPT_H 1
#define HAVE_CTIME 1
#define HAVE_CTYPE_H 1
#define HAVE_CURSES_H 1
#define HAVE_CUSERID 1
#define HAVE_CXXABI_H 1
#define HAVE_DECL_BZERO 1
#define HAVE_DECL_FDATASYNC 1
#define HAVE_DECL_MADVISE 1
#define HAVE_DECL_NS_T_INVALID 1
#define HAVE_DECL_NS_T_SPF 0
#define HAVE_DECL_RES_NDESTROY 0
#define HAVE_DECL_RES_NINIT 1
#define HAVE_DECL_SHM_HUGETLB 1
#define HAVE_DECL_TGOTO 1
#define HAVE_DIFFTIME 1
#define HAVE_DIRENT_H 1
#define HAVE_DLERROR 1
#define HAVE_DLFCN_H 1
#define HAVE_DLOPEN
#define HAVE_ERRNO_H 1
#define HAVE_EVENT_BASE_FREE 1
#define HAVE_EVENT_BASE_GET_METHOD 1
#define HAVE_EVENT_BASE_NEW 1
#define HAVE_EXECINFO_H 1
#define HAVE_EXPLICIT_TEMPLATE_INSTANTIATION 1
#define HAVE_FCHMOD 1
#define HAVE_FCNTL 1
#define HAVE_FCNTL_H 1
#define HAVE_FDATASYNC 1
#define HAVE_FEDISABLEEXCEPT 1
#define HAVE_FENV_H 1
#define HAVE_FESETROUND 1
#define HAVE_FINITE 1
#define HAVE_FLOAT_H 1
#define HAVE_FLOCKFILE 1
#define HAVE_FLOORF 1
#define HAVE_FNMATCH_H 1
#define HAVE_FORK 1
#define HAVE_FPRINTF 1
#define HAVE_FPU_CONTROL_H 1
#define HAVE_FSTAT 1
#define HAVE_FSYNC 1
#define HAVE_FT2BUILD_H 1
#define HAVE_FTIME 1
#define HAVE_FTRUNCATE 1
#define HAVE_GCC_ATOMIC_BUILTINS 1
#define HAVE_GETADDRINFO
#define HAVE_GETCWD 1
#define HAVE_GETEGID 1
#define HAVE_GETENV 1
#define HAVE_GETEUID 1
#define HAVE_GETGID 1
#define HAVE_GETHOSTBYADDR_R 1
#define HAVE_GETHOSTBYNAME 1
#define HAVE_GETHOSTBYNAME_R 1
#define HAVE_GETHOSTBYNAME_R_GLIBC2_STYLE 1
#define HAVE_GETHOSTNAME 1
#define HAVE_GETLINE 1
#define HAVE_GETOPT_H 1
#define HAVE_GETOPT_LONG_ONLY 1
#define HAVE_GETPAGESIZE 1
#define HAVE_GETPASS 1
#define HAVE_GETPWNAM 1
#define HAVE_GETPWUID 1
#define HAVE_GETRLIMIT 1
#define HAVE_GETRUSAGE 1
#define HAVE_GETTIMEOFDAY 1
#define HAVE_GETUID 1
#define HAVE_GETWD 1
#define HAVE_GMTIME 1
#define HAVE_GMTIME_R 1
#define HAVE_GRP_H 1
#define HAVE_IB_ATOMIC_PTHREAD_T_GCC 1
#define HAVE_IB_GCC_ATOMIC_BUILTINS 1
#define HAVE_IB_PAUSE_INSTRUCTION 1
#define HAVE_ICONV 1
#define HAVE_ICONV_H 1
#define HAVE_ICONV_T_DEF 1
#define HAVE_IN_ADDR_T 1
#define HAVE_INDEX 1
#define HAVE_INET_NTOA 1
#define HAVE_INITGROUPS 1
#define HAVE_INTTYPES_H 1
#define HAVE_ISATTY 1
#define HAVE_ISINF 1
#define HAVE_ISNAN 1
#define HAVE_ISWCTYPE 1
#define HAVE_ISWLOWER 1
#define HAVE_ISWUPPER 1
#define HAVE_LANGINFO_CODESET
#define HAVE_LANGINFO_H 1
#define HAVE_LARGE_PAGES 1
#define HAVE_LIBDL 1
#define HAVE_LIBEVENT 1
#define HAVE_LIBFREETYPE 1
#define HAVE_LIBINTL_H 1
#define HAVE_LIBJPEG 1
#define HAVE_LIBM 1
#define HAVE_LIBMEMCACHED 1
#define HAVE_LIBMEMCACHEDUTIL 1
#define HAVE_LIBNSL 1
#define HAVE_LIBPNG 1
#define HAVE_LIBPTHREAD 1
#define HAVE_LIBZ 1
#define HAVE_LIMITS_H 1
#define HAVE_LOCALE_H 1
#define HAVE_LOCALTIME 1
#define HAVE_LOCALTIME_R 1
#define HAVE_LONGJMP 1
#define HAVE_LONG_LONG_INT 1
#define HAVE_LRAND48 1
#define HAVE_LSTAT 1
#define HAVE_MADVISE 1
#define HAVE_MALLINFO 1
#define HAVE_MALLOC 1
#define HAVE_MALLOC_H 1
#define HAVE_MATH_H 1
#define HAVE_MBRLEN
#define HAVE_MBRTOWC
#define HAVE_MBSRTOWCS
#define HAVE_MBSTATE_T
#define HAVE_MEMCMP 1
#define HAVE_MEMCPY 1
#define HAVE_MEMMOVE 1
#define HAVE_MEMORY_H 1
#define HAVE_MEMSET 1
#define HAVE_MKDIR 1
#define HAVE_MKSTEMP 1
#define HAVE_MKTIME 1
#define HAVE_MLOCKALL 1
#define HAVE_MMAP 1
#define HAVE_MMAP64 1
#define HAVE_MODE_T
#define HAVE_MPROTECT 1
#define HAVE_MSG_DONTWAIT 1
#define HAVE_MSG_MORE 1
#define HAVE_MSG_NOSIGNAL 1
#define HAVE_MUNMAP 1
#define HAVE_MURMUR_HASH 1
#define HAVE_NETDB_H 1
#define HAVE_NETINET_IN_H 1
#define HAVE_NPTL 1
#define HAVE_OFF_T 1
#define HAVE_PATHS_H 1
#define HAVE_PERROR 1
#define HAVE_PNG_H 1
#define HAVE_POLL 1
#define HAVE_POLL_H 1
#define HAVE_POSIX_FALLOCATE 1
#define HAVE_POSIX_SIGNALS 1
#define HAVE_PREAD 1
#define HAVE_PRINTF 1
#define HAVE_PTHREAD 1
#define HAVE_PTHREAD_ATTR_GETSTACKSIZE 1
#define HAVE_PTHREAD_ATTR_SETSCHEDPARAM 1
#define HAVE_PTHREAD_ATTR_SETSCOPE 1
#define HAVE_PTHREAD_ATTR_SETSTACKSIZE 1
#define HAVE_PTHREAD_H 1
#define HAVE_PTHREAD_KEY_DELETE 1
#define HAVE_PTHREAD_RWLOCK_RDLOCK 1
#define HAVE_PTHREAD_SETSCHEDPARAM 1
#define HAVE_PTHREAD_SETSCHEDPRIO 1
#define HAVE_PTHREAD_SIGMASK 1
#define HAVE_PTHREAD_YIELD_ZERO_ARG 1
#define HAVE_PUTENV 1
#define HAVE_PWD_H 1
#define HAVE_QSORT 1
#define HAVE_QUERY_CACHE 1
#define HAVE_RAISE 1
#define HAVE_RCVTIMEO 1
#define HAVE_READDIR_R 1
#define HAVE_READLINK 1
#define HAVE_REALLOC 1
#define HAVE_REALPATH 1
#define HAVE_RE_COMP 1
#define HAVE_REGCOMP 1
#define HAVE_RENAME 1
#define HAVE_RESOLV_H 1
#define HAVE_RESTARTABLE_SYSCALLS 1
#define HAVE_RINT 1
#define HAVE_RMDIR 1
#define HAVE_RTREE_KEYS 1
#define HAVE_SCHED_H 1
#define HAVE_SCHED_YIELD 1
#define HAVE_SELECT 1
#define HAVE_SEMAPHORE_H 1
#define HAVE_SETENV 1
#define HAVE_SETJMP 1
#define HAVE_SETJMP_H 1
#define HAVE_SETLOCALE 1
#define HAVE_SHMAT 1
#define HAVE_SHMCTL 1
#define HAVE_SHMDT 1
#define HAVE_SHMGET 1
#define HAVE_SIGACTION 1
#define HAVE_SIGADDSET 1
#define HAVE_SIGEMPTYSET 1
#define HAVE_SIGHOLD 1
#define HAVE_SIGNAL 1
#define HAVE_SIGNAL_H 1
#define HAVE_SIGSET 1
#define HAVE_SIGSET_T 1
#define HAVE_SIGWAIT 1
#define HAVE_SIZE_T 1
#define HAVE_SLEEP 1
#define HAVE_SNDTIMEO 1
#define HAVE_SNPRINTF 1
#define HAVE_SOCKET 1
#define HAVE_SPATIAL 1
#define HAVE_SPRINTF 1
#define HAVE_SSCANF 1
#define HAVE_STAT 1
#define HAVE_STDARG_H 1
#define HAVE_STDDEF_H 1
#define HAVE_STDINT_H 1
#define HAVE_STDIO_H 1
#define HAVE_STDLIB_H 1
#define HAVE_STPCPY 1
#define HAVE_STRCASECMP 1
#define HAVE_STRCHR 1
#define HAVE_STRCOLL 1
#define HAVE_STRCSPN 1
#define HAVE_ST_RDEV 1
#define HAVE_STRDUP 1
#define HAVE_STRERROR 1
#define HAVE_STRERROR_S 1
#define HAVE_STRFTIME 1
#define HAVE_STRING_H 1
#define HAVE_STRINGS_H 1
#define HAVE_STRNCASECMP 1
#define HAVE_STRNDUP 1
#define HAVE_STRNLEN 1
#define HAVE_STRPBRK 1
#define HAVE_STRRCHR 1
#define HAVE_STRSIGNAL 1
#define HAVE_STRSPN 1
#define HAVE_STRSTR 1
#define HAVE_STRTOK_R 1
#define HAVE_STRTOL 1
#define HAVE_STRTOLL 1
#define HAVE_STRTOUL 1
#define HAVE_STRTOULL 1
#define HAVE_STRUCT_IN6_ADDR 1
#define HAVE_STRUCT_STAT_ST_RDEV 1
#define HAVE_STRUCT_TIMESPEC
#define HAVE_SYS_CDEFS_H 1
#define HAVE_SYS_DIR_H 1
#define HAVE_SYS_FILE_H 1
#define HAVE_SYS_IOCTL_H 1
#define HAVE_SYS_IPC_H 1
#define HAVE_SYSLOG_H 1
#define HAVE_SYS_MMAN_H 1
#define HAVE_SYS_PARAM_H 1
#define HAVE_SYS_PRCTL_H 1
#define HAVE_SYS_RESOURCE_H 1
#define HAVE_SYS_SELECT_H 1
#define HAVE_SYS_SHM_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_SYS_TIMEB_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_UN_H 1
#define HAVE_SYS_WAIT_H 1
#define HAVE_TCGETATTR 1
#define HAVE_TEMPNAM 1
#define HAVE_TERMCAP_H 1
#define HAVE_TERM_H 1
#define HAVE_TERMIO_H 1
#define HAVE_TERMIOS_H 1
#define HAVE_TIME 1
#define HAVE_TIME_H 1
#define HAVE_TOWLOWER 1
#define HAVE_TOWUPPER 1
#define HAVE_TZNAME 1
#define HAVE_U_CHAR 1
#define HAVE_UCONTEXT_H 1
#define HAVE_UINT 1
#define HAVE_U_INT16_T 1
#define HAVE_UINT16_T 1
#define HAVE_U_INT32_T 1
#define HAVE_UINT32_T 1
#define HAVE_U_INT8_T 1
#define HAVE_UINT8_T 1
#define HAVE_ULONG 1
#define HAVE_UMASK 1
#define HAVE_UNISTD_H 1
#define HAVE_UTIME 1
#define HAVE_UTIME_H 1
#define HAVE_UTIME_NULL 1
#define HAVE_VA_COPY 1
#define HAVE_VFORK 1
#define HAVE_VFPRINTF 1
#define HAVE_VIO_READ_BUFF 1
#define HAVE_VISIBILITY 1
#define HAVE_VPRINTF 1
#define HAVE_VSNPRINTF 1
#define HAVE_VSPRINTF 1
#define HAVE_WCHAR_H 1
#define HAVE_WCHAR_T 1
#define HAVE_WCRTOMB
#define HAVE_WCSCOLL
#define HAVE_WCSDUP
#define HAVE_WCTYPE
#define HAVE_WCTYPE_H 1
#define HAVE_WCTYPE_T 1
#define HAVE_WCWIDTH
#define HAVE_WEAK_SYMBOL 1
#define HAVE_WORKING_FORK 1
#define HAVE_WORKING_VFORK 1
#define HAVE_ZLIB_H 1

 */

#endif
