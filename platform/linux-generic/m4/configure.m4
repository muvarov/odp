ODP_IMPLEMENTATION_NAME="odp-linux"

ODP_VISIBILITY
ODP_ATOMIC

ODP_CHECK_CFLAG([-pthread])
PTHREAD_LIBS_LIBODP="-lpthread"
AC_SUBST([PTHREAD_LIBS_LIBODP])

ODP_TIMER
ODP_OPENSSL
ODP_LIBCONFIG([linux-generic])
m4_include([platform/linux-generic/m4/odp_pcap.m4])
m4_include([platform/linux-generic/m4/odp_pcapng.m4])
m4_include([platform/linux-generic/m4/odp_netmap.m4])
m4_include([platform/linux-generic/m4/odp_dpdk.m4])
m4_include([platform/linux-generic/m4/odp_zlib.m4])
ODP_SCHEDULER

m4_include([platform/linux-generic/m4/performance.m4])

AC_CONFIG_COMMANDS_PRE([dnl
AM_CONDITIONAL([PLATFORM_IS_LINUX_GENERIC],
	       [test "${with_platform}" = "linux-generic"])
AC_CONFIG_FILES([platform/linux-generic/Makefile
		 platform/linux-generic/libodp-linux.pc
		 platform/linux-generic/test/Makefile
		 platform/linux-generic/test/validation/api/shmem/Makefile
		 platform/linux-generic/test/validation/api/pktio/Makefile
		 platform/linux-generic/test/mmap_vlan_ins/Makefile
		 platform/linux-generic/test/pktio_ipc/Makefile
		 platform/linux-generic/test/ring/Makefile
		 platform/linux-generic/test/performance/Makefile])
])
