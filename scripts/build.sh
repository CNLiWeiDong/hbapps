#!/bin/bash
##########################################################################
   #"$BASH_SOURCE[0]" == $0
   SOURCE_DIR="$( cd "$( dirname "$0" )" && pwd )"
   function usage()
   {
      printf "\\tUsage: %s \\n\\t[Build Option -o <Debug|Release|RelWithDebInfo|MinSizeRel>]\\n\\t[Avoid Compiling -a]\\n\\n" "$0" 1>&2
      exit 1
   }
   ARCH=$( uname )
   if [ "${SOURCE_DIR}" == "${PWD}" ]; then
      BUILD_DIR="${PWD}/build"
   else
      BUILD_DIR="${PWD}"
   fi
   CMAKE_BUILD_TYPE=Release
   START_MAKE=true
   TIME_BEGIN=$( date -u +%s )
   VERSION=1.0

   txtrst=$(tput sgr0)
	txtred=${txtrst}$(tput setaf 1)
	txtgreen=${txtrst}$(tput setaf 2)
	txtbld=$(tput bold)
	bldred=${txtbld}$(tput setaf 1)
	bldgreen=${txtbld}$(tput setaf 2)

   if [ $# -ne 0 ]; then
      while getopts ":cdo:ah" opt; do
         case "${opt}" in
            o )
               options=( "Debug" "Release" "RelWithDebInfo" "MinSizeRel" )
               if [[ "${options[*]}" =~ "${OPTARG}" ]]; then
                  CMAKE_BUILD_TYPE="${OPTARG}"
               else
                  printf "\\n\\tInvalid argument: %s\\n" "${OPTARG}" 1>&2
                  usage
                  exit 1
               fi
            ;;
            a)
               START_MAKE=false
            ;;
            h)
               usage
               exit 1
            ;;
            \? )
               printf "\\n\\tInvalid Option: %s\\n" "-${OPTARG}" 1>&2
               usage
               exit 1
            ;;
            : )
               printf "\\n\\tInvalid Option: %s requires an argument.\\n" "-${OPTARG}" 1>&2
               usage
               exit 1
            ;;
            * )
               usage
               exit 1
            ;;
         esac
      done
   fi

   if [ ! -d "${SOURCE_DIR}/.git" ]; then
      printf "\\n\\tThis build script only works with sources cloned from git\\n"
      printf "\\tPlease clone a new app directory with 'git clone https://github.com/ --recursive'\\n"
      exit 1
   fi
   pushd "${SOURCE_DIR}" &> /dev/null

   STALE_SUBMODS=$(( $(git submodule status --recursive | grep -c "^[+\-]") ))
   if [ $STALE_SUBMODS -gt 0 ]; then
      printf "\\n\\tgit submodules are not up to date.\\n"
      printf "\\tPlease run the command 'git submodule update --init --recursive'.\\n"
      exit 1
   fi

   printf "${txtrst}\\n\\tBeginning build version: %s\\n" "${VERSION}"
   printf "\\t%s\\n" "$( date -u )"
   printf "\\tUser: %s\\n" "$( whoami )"
   printf "\\tgit head id: %s\\n" "$( cat .git/refs/heads/master )"
   printf "\\tCurrent branch: %s\\n" "$( git rev-parse --abbrev-ref HEAD )"
	printf "\\tSource dir: %s\\n" "${SOURCE_DIR}"
	printf "\\tBuild dir: %s\\n\\n" "${BUILD_DIR}"
   printf "\\tARCHITECTURE: %s\\n\\n" "${ARCH}"

   popd &> /dev/null

   if [ "$ARCH" == "Linux" ]; then

      if [ ! -e /etc/os-release ]; then
         printf "\\n\\tAPP currently supports Amazon, Centos, Fedora, Mint & Ubuntu Linux only.\\n"
         printf "\\tPlease install on the latest version of one of these Linux distributions.\\n"
         printf "\\thttps://aws.amazon.com/amazon-linux-ami/\\n"
         printf "\\thttps://www.centos.org/\\n"
         printf "\\thttps://start.fedoraproject.org/\\n"
         printf "\\thttps://linuxmint.com/\\n"
         printf "\\thttps://www.ubuntu.com/\\n"
         printf "\\tExiting now.\\n"
         exit 1
      fi

      OS_NAME=$( cat /etc/os-release | grep ^NAME | cut -d'=' -f2 | sed 's/\"//gI' )

      case "$OS_NAME" in
         "Amazon Linux AMI"|"Amazon Linux")
            FILE="${SOURCE_DIR}/scripts/build_amazon.sh"
            CXX_COMPILER=g++
            C_COMPILER=gcc
         ;;
         "CentOS Linux")
            FILE="${SOURCE_DIR}/scripts/build_centos.sh"
            CXX_COMPILER=g++
            C_COMPILER=gcc
         ;;
         "elementary OS")
            FILE="${SOURCE_DIR}/scripts/build_ubuntu.sh"
            CXX_COMPILER=clang++-4.0
            C_COMPILER=clang-4.0
         ;;
         "Fedora")
            FILE="${SOURCE_DIR}/scripts/build_fedora.sh"
            CXX_COMPILER=g++
            C_COMPILER=gcc
         ;;
         "Linux Mint")
            FILE="${SOURCE_DIR}/scripts/build_ubuntu.sh"
            CXX_COMPILER=clang++-4.0
            C_COMPILER=clang-4.0
         ;;
         "Ubuntu")
            FILE="${SOURCE_DIR}/scripts/build_ubuntu.sh"
            CXX_COMPILER=clang++-4.0
            C_COMPILER=clang-4.0
         ;;
         "Debian GNU/Linux")
            FILE=${SOURCE_DIR}/scripts/build_ubuntu.sh
            CXX_COMPILER=clang++-4.0
            C_COMPILER=clang-4.0
         ;;
         *)
            printf "\\n\\tUnsupported Linux Distribution. Exiting now.\\n\\n"
            exit 1
      esac
   fi

   if [ "$ARCH" == "Darwin" ]; then
      FILE="${SOURCE_DIR}/scripts/build_darwin.sh"
      CXX_COMPILER=clang++
      C_COMPILER=clang
   fi

   pushd "${SOURCE_DIR}" &> /dev/null
   . "$FILE"
   popd &> /dev/null

   printf "${bldgreen}\\n\\n>>>>>>>> ALL dependencies sucessfully found or installed . Installing APP\\n\\n"
   printf "${bldgreen}>>>>>>>> CMAKE_BUILD_TYPE=%s\\n" "${CMAKE_BUILD_TYPE}"
   printf "${bldgreen}>>>>>>>> START_MAKE=%s\\n" "${START_MAKE}"
   printf "${txtrst}"

   if [ ! -d "${BUILD_DIR}" ]; then
      if ! mkdir -p "${BUILD_DIR}"
      then
         printf "Unable to create build directory %s.\\n Exiting now.\\n" "${BUILD_DIR}"
         exit 1;
      fi
   fi

   if ! cd "${BUILD_DIR}"
   then
      printf "Unable to enter build directory %s.\\n Exiting now.\\n" "${BUILD_DIR}"
      exit 1;
   fi

   if [ -z "$CMAKE" ]; then
      CMAKE=$( command -v cmake )
   fi
  
   if ! "${CMAKE}" ${SOURCE_DIR} -DCMAKE_BUILD_TYPE="${CMAKE_BUILD_TYPE}" -DCMAKE_CXX_COMPILER="${CXX_COMPILER}" \
      -DCMAKE_C_COMPILER="${C_COMPILER}"
   then
      printf "\\n\\t>>>>>>>>>>>>>>>>>>>> CMAKE building APP has exited with the above error.\\n\\n"
      exit -1
   fi

   if [ "${START_MAKE}" == "false" ]; then
      printf "\\n\\t>>>>>>>>>>>>>>>>>>>> APP has been successfully configured but not yet built.\\n\\n"
      exit 0
   fi

   if [ -z ${JOBS} ]; then JOBS=$CPU_CORE; fi
   if ! make -j "${JOBS}"
   then
      printf "\\n\\t>>>>>>>>>>>>>>>>>>>> MAKE building APP has exited with the above error.\\n\\n"
      exit -1
   fi

   TIME_END=$(( $(date -u +%s) - ${TIME_BEGIN} ))
   printf "\t\n${txtrst}"
   printf "\\n\\tAPP has been successfully built. %02d:%02d:%02d\\n\\n" $(($TIME_END/3600)) $(($TIME_END%3600/60)) $(($TIME_END%60))
