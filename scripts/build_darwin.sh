	ARCH=$( uname )
	OS_VER=$(sw_vers -productVersion)
	OS_MAJ=$(echo "${OS_VER}" | cut -d'.' -f1)
	OS_MIN=$(echo "${OS_VER}" | cut -d'.' -f2)
	OS_PATCH=$(echo "${OS_VER}" | cut -d'.' -f3)
	MEM_GIG=$(bc <<< "($(sysctl -in hw.memsize) / 1024000000)")
	CPU_SPEED=$(bc <<< "scale=2; ($(sysctl -in hw.cpufrequency) / 10^8) / 10")
	CPU_CORE=$( sysctl -in machdep.cpu.core_count )
	DISK_INSTALL=$(df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 || cut -d' ' -f1)
	blksize=$(df . | head -1 | awk '{print $2}' | cut -d- -f1)
	gbfactor=$(( 1073741824 / blksize ))
	total_blks=$(df . | tail -1 | awk '{print $2}')
	avail_blks=$(df . | tail -1 | awk '{print $4}')
	DISK_TOTAL=$((total_blks / gbfactor ))
	DISK_AVAIL=$((avail_blks / gbfactor ))
	DISK_MIN=20
	
	
	
	printf "${txtrst}\\n\\tOS name: %s\\n" "${ARCH}"
	printf "${txtrst}\\tOS Version: %s\\n" "${OS_VER}"
	printf "${txtrst}\\tCPU speed: %sGhz\\n" "${CPU_SPEED}"
	printf "${txtrst}\\tCPU cores: %s\\n" "${CPU_CORE}"
	printf "${txtrst}\\tPhysical Memory: %s Gbytes\\n" "${MEM_GIG}"
	printf "${txtrst}\\tDisk install: %s\\n" "${DISK_INSTALL}"
	printf "${txtrst}\\tDisk space total: %sG\\n" "${DISK_TOTAL}"
	printf "${txtrst}\\tDisk space available: %sG\\n" "${DISK_AVAIL}"

	printf "${bldgreen}\\n\\tChecking Home Brew installation\\n"
	if ! BREW=$( command -v brew )
	then
		printf "${bldred}\\tHomebrew must be installed to compile App\\n\\n"
		exit 1
	fi

	# cmake install
	DEP="cmake"
	if [ ! -x "/usr/local/bin/cmake" ]; then
		printf "${bldgreen}\\t\\t Installing %s\\n" "${DEP}"
		if ! "${BREW}" install --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1
		fi
		if ! "${BREW}" unlink ${DEP} && "${BREW}" link --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1
		fi
	fi
	printf "${bldgreen}\\t\\t Installed %s\\n" "${DEP}"
	# llvm install 
	DEP="llvm"
	if [ ! -x "/usr/local/opt/llvm/bin/clang" ]; then
		printf "${bldgreen}\\t\\t Installing %s\\n" "${DEP}"
		if ! "${BREW}" install --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1
		fi
		if ! "${BREW}" unlink ${DEP} && "${BREW}" link --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
	fi
	printf "${bldgreen}\\t\\t Installed %s\\n" "${DEP}"
	# openssl@1.1 install
	DEP="openssl@1.1"
	if [ ! -f "/usr/local/opt/openssl@1.1/lib/libssl.a" ]; then
		printf "${bldgreen}\\t\\t Installing %s\\n" "${DEP}"
		if ! "${BREW}" install --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! "${BREW}" unlink ${DEP} && "${BREW}" link --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
	fi
	printf "${bldgreen}\\t\\t Installed %s\\n" "${DEP}"
	# boost install
	DEP="boost"
	if [ ! -f "/usr/local/opt/boost/include/boost/version.hpp" ]; then
		printf "${bldgreen}\\t\\t Installing %s\\n" "${DEP}"
		if ! "${BREW}" install --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! "${BREW}" unlink ${DEP} && "${BREW}" link --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
	fi
	printf "${bldgreen}\\t\\t Installed %s\\n" "${DEP}"
	# mysql-client install
	DEP="mysql-client"
	if [ ! -x "/usr/local/opt/mysql-client/bin/mysqlcheck" ]; then
		printf "\\t\\t Installing %s\\n" "${DEP}"
		if ! "${BREW}" install --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! "${BREW}" unlink ${DEP} && "${BREW}" link --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
	fi
	printf "${bldgreen}\\t\\t Installed %s\\n" "${DEP}"
	# mysql++ install
	DEP="mysql++"
	if [ ! -d "/usr/local/opt/mysql++" ]; then
		printf "\\t\\t Installing %s\\n" "${DEP}"
		if ! "${BREW}" install --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
		if ! "${BREW}" unlink ${DEP} && "${BREW}" link --force ${DEP}
		then
			printf "${bldred}\\tHomebrew exited with the above errors.%s\\n" "${DEP}"
			printf "${bldred}\\tExiting now.\\n\\n"
			exit 1;
		fi
	fi
	printf "${bldgreen}\\t\\t Installed %s\\n" "${DEP}"
	printf "${txtrst}"



