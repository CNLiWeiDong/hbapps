	OS_VER=$( grep VERSION_ID /etc/os-release | cut -d'=' -f2 | sed 's/[^0-9\.]//gI' )
	OS_MAJ=$(echo "${OS_VER}" | cut -d'.' -f1)
	OS_MIN=$(echo "${OS_VER}" | cut -d'.' -f2)

	MEM_MEG=$( free -m | sed -n 2p | tr -s ' ' | cut -d\  -f2 || cut -d' ' -f2 )
	CPU_SPEED=$( lscpu | grep -m1 "MHz" | tr -s ' ' | cut -d\  -f3 || cut -d' ' -f3 | cut -d'.' -f1 )
	CPU_CORE=$( lscpu -pCPU | grep -v "#" | wc -l )

	MEM_GIG=$(( ((MEM_MEG / 1000) / 2) ))
	# JOBS=$(( MEM_GIG > CPU_CORE ? CPU_CORE : MEM_GIG ))
	JOBS=$CPU_CORE

	DISK_INSTALL=$(df -h . | tail -1 | tr -s ' ' | cut -d\  -f1 || cut -d' ' -f1)
	DISK_TOTAL_KB=$(df . | tail -1 | awk '{print $2}')
	DISK_AVAIL_KB=$(df . | tail -1 | awk '{print $4}')
	DISK_TOTAL=$(( DISK_TOTAL_KB / 1048576 ))
	DISK_AVAIL=$(( DISK_AVAIL_KB / 1048576 ))

	printf "\\n\\tOS name: %s\\n" "${OS_NAME}"
	printf "\\tOS Version: %s\\n" "${OS_VER}"
	printf "\\tCPU speed: %sMhz\\n" "${CPU_SPEED}"
	printf "\\tCPU cores: %s\\n" "${CPU_CORE}"
	printf "\\tPhysical Memory: %s Mgb\\n" "${MEM_MEG}"
	printf "\\tDisk install: %s\\n" "${DISK_INSTALL}"
	printf "\\tDisk space total: %sG\\n" "${DISK_TOTAL%.*}"
	printf "\\tDisk space available: %sG\\n" "${DISK_AVAIL%.*}"

	if [ "${MEM_MEG}" -lt 1500 ]; then
		printf "\\tYour system must have 1.5 or more Gigabytes of physical memory installed.\\n"
		printf "\\tExiting now.\\n"
		exit 1
	fi

	case "${OS_NAME}" in
		"Linux Mint")
		   if [ "${OS_MAJ}" -lt 18 ]; then
			   printf "\\tYou must be running Linux Mint 18.x or higher to install APP.\\n"
			   printf "\\tExiting now.\\n"
			   exit 1
		   fi
		;;
		"Ubuntu")
			if [ "${OS_MAJ}" -lt 16 ]; then
				printf "\\tYou must be running Ubuntu 16.04.x or higher to install APP.\\n"
				printf "\\tExiting now.\\n"
				exit 1
			fi
		;;
		"Debian")
			if [ $OS_MAJ -lt 10 ]; then
				printf "\tYou must be running Debian 10 to install APP, and resolve missing dependencies from unstable (sid).\n"
				printf "\tExiting now.\n"
				exit 1
		fi
		;;
	esac

	if [ "${DISK_AVAIL%.*}" -lt "${DISK_MIN}" ]; then
		printf "\\tYou must have at least %sGB of available storage to install APP.\\n" "${DISK_MIN}"
		printf "\\tExiting now.\\n"
		exit 1
	fi

	#lldb-4.0 libclang-4.0-dev python2.7-dev python3-dev libicu-dev zlib1g-dev  graphviz libbz2-dev autotools-dev
	#build-essential c++编译包，包含gcc g++ make pkg等
	#llvm-4.0 clang-4.0 clang环境需要llvm虚拟机和clang编译器
	DEP_ARRAY=(build-essential llvm-4.0 clang-4.0 cmake make automake autoconf libtool pkg-config doxygen curl \
	libgmp3-dev libssl-dev libjsoncpp-dev libprotobuf-dev protobuf-compiler libcurl4-openssl-dev \
	liblog4cplus-dev \
    )

	COUNT=1
	DISPLAY=""
	DEP=""

	if [[ "${ENABLE_CODE_COVERAGE}" == true ]]; then
		DEP_ARRAY+=(lcov)
	fi

	printf "\\n\\tChecking for installed dependencies.\\n\\n"

	for (( i=0; i<${#DEP_ARRAY[@]}; i++ ));
	do
		pkg=$( dpkg -s "${DEP_ARRAY[$i]}" 2>/dev/null | grep Status | tr -s ' ' | cut -d\  -f4 )
		if [ -z "$pkg" ]; then
			DEP=$DEP" ${DEP_ARRAY[$i]} "
			DISPLAY="${DISPLAY}${COUNT}. ${DEP_ARRAY[$i]}\\n\\t"
			printf "\\tPackage %s ${bldred} NOT ${txtrst} found.\\n" "${DEP_ARRAY[$i]}"
			(( COUNT++ ))
		else
			printf "\\tPackage %s found.\\n" "${DEP_ARRAY[$i]}"
			continue
		fi
	done		

	if [ "${COUNT}" -gt 1 ]; then
		printf "\\n\\tThe following dependencies are required to install APP.\\n"
		printf "\\n\\t${DISPLAY}\\n\\n" 
		printf "\\tDo you wish to install these packages?\\n"
		select yn in "Yes" "No"; do
			case $yn in
				[Yy]* ) 
					printf "\\n\\n\\tInstalling dependencies\\n\\n"
					sudo apt-get update
					if ! sudo apt-get -y install ${DEP}
					then
						printf "\\n\\tDPKG dependency failed.\\n"
						printf "\\n\\tExiting now.\\n"
						exit 1
					else
						printf "\\n\\tDPKG dependencies installed successfully.\\n"
					fi
				break;;
				[Nn]* ) echo "User aborting installation of required dependencies, Exiting now."; exit;;
				* ) echo "Please type 1 for yes or 2 for no.";;
			esac
		done
	else 
		printf "\\n\\tNo required dpkg dependencies to install.\\n"
	fi

	export BOOST_VERSION_MAJOR=1
	export BOOST_VERSION_MINOR=67
	export BOOST_VERSION_PATCH=0
	export BOOST_VERSION=${BOOST_VERSION_MAJOR}_${BOOST_VERSION_MINOR}_${BOOST_VERSION_PATCH}
	export BOOST_ROOT="${HOME}/opt/boost"

	printf "Checking Boost library (${BOOST_VERSION}) installation...\\n"
	BOOSTVERSION=$( grep "#define BOOST_VERSION" "$HOME/opt/boost/include/boost/version.hpp" 2>/dev/null | tail -1 | tr -s ' ' | cut -d\  -f3 )
	if [ "${BOOSTVERSION}" != "${BOOST_VERSION_MAJOR}0${BOOST_VERSION_MINOR}0${BOOST_VERSION_PATCH}" ]; then
		printf "Installing Boost library...\\n"
		pushd "$(pwd)" &> /dev/null
		cd $TEMP_DIR \
		&& curl -LO https://dl.bintray.com/boostorg/release/${BOOST_VERSION_MAJOR}.${BOOST_VERSION_MINOR}.${BOOST_VERSION_PATCH}/source/boost_$BOOST_VERSION.tar.bz2 \
		&& tar -xjf boost_$BOOST_VERSION.tar.bz2 \
		&& cd boost_${BOOST_VERSION} \
		&& ./bootstrap.sh --prefix=$BOOST_ROOT \
		&& ./b2 -q -j"${CPU_CORE}" install \
		&& cd .. \
		&& rm -f boost_$BOOST_VERSION.tar.bz2 \
		&& rm -rf boost_${BOOST_VERSION} \
		|| exit 1
		popd &> /dev/null
		printf " - Boost library successfully installed @ ${BOOST_ROOT} (Symlinked to ${BOOST_ROOT}).\\n"
	else
		printf " - Boost library found with correct version @ ${BOOST_ROOT} (Symlinked to ${BOOST_ROOT}).\\n"
	fi
	if [ $? -ne 0 ]; then exit -1; fi

	function print_instructions()
	{
		printf "\\tcd %s; make test\\n\\n" "${BUILD_DIR}"
	return 0
	}
