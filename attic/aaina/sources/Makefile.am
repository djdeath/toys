noinst_LTLIBRARIES = libsources.la

INCLUDES = \
	$(DEPS_CFLAGS)
	-I$(top_srcdir) 				\
	-I$(top_builddir) 				\
	$(GCC_CFLAGS)					\
	-DDATADIR=\""$(datadir)"\" 			\
	-DSYSCONFDIR=\""$(sysconfdir)"\" 		\
	-Werror						\
	$(NULL)	

libsources_la_SOURCES =	\
	aaina-source-directory.c			\
	aaina-source-directory.h			\
	aaina-source-flickr.c				\
	aaina-source-flickr.h				

libsources_la_LIBADD =	\
	$(DEPS_LIBS)	 				\
	$(top_builddir)/libaaina/libaaina.la		\
	$(top_builddir)/libnflick/libnflick.la	

libsources_la_LDFLAGS = -version-info 0:1:0
