#
#  CMakeLists.txt
#  leveldb(windows)
#
#  Created by kimbomm on 2018. 09. 25...
#  Copyright 2018 kimbomm. All rights reserved.
#
# Copyright 2017 The LevelDB Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file. See the AUTHORS file for names of contributors.

cmake_minimum_required(VERSION 3.9)
project(leveldb_wbd VERSION 1.21.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option (BUILD_SHARED_LIBS "Build shared libraries" OFF)

message("================================================================================")
message("                      leveldb needs a boost library.                            ")
message("                   Build leveldb after boost installation.                      ")
message("================================================================================")

set(BOOST_VER boost_1_68_0)

#message(${CMAKE_VS_PLATFORM_NAME})	#Win32 x64


file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/boost_build/)
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/lib/)
file(DOWNLOAD https://github.com/springkim/leveldb/releases/download/bin/7z.dll ${PROJECT_SOURCE_DIR}/boost_build/7z.dll)
file(DOWNLOAD https://github.com/springkim/leveldb/releases/download/bin/7z.exe ${PROJECT_SOURCE_DIR}/boost_build/7z.exe)
file(DOWNLOAD https://dl.bintray.com/boostorg/release/1.68.0/source/${BOOST_VER}.7z ${PROJECT_SOURCE_DIR}/boost_build/${BOOST_VER}.7z SHOW_PROGRESS)
execute_process(COMMAND cmd /c "7z.exe x ${BOOST_VER}.7z -y" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/boost_build")


if(MSVC)
	if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "Win32")
		set(ADDRESS_MODEL 32)
	else()
		set(ADDRESS_MODEL 64)
	endif()
	
	
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013")
		set(TOOLSET msvc-12.0)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
		set(TOOLSET msvc-12.0)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015")
		set(TOOLSET msvc-14.0)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(TOOLSET msvc-14.0)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017")
		set(TOOLSET msvc-14.1)
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017 Win64")
		set(TOOLSET msvc-14.1)
	endif()
endif(MSVC)
if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
	set(ADDRESS_MODEL 64)
	set(TOOLSET gcc)
endif(MSYS)

execute_process(COMMAND cmd /c "call bootstrap.bat" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/boost_build/${BOOST_VER}")
execute_process(COMMAND cmd /c "b2.exe --toolset=${TOOLSET} variant=debug,release address-model=${ADDRESS_MODEL} threading=multi link=static runtime-link=shared --with-date_time --with-system --with-filesystem --with-thread" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/boost_build/${BOOST_VER}")



execute_process(COMMAND cmd /c "xcopy /Y boost\\*.* ..\\..\\include\\boost\\ /e /h /k 2>&1 >NUL" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/boost_build/${BOOST_VER}")
execute_process(COMMAND cmd /c "xcopy /Y stage\\lib\\*.* ..\\..\\lib\\ /e /h /k 2>&1 >NUL" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/boost_build/${BOOST_VER}")


execute_process(COMMAND cmd /c "dir *_chrono*d-*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_CHRONO_d)
execute_process(COMMAND cmd /c "dir *_date_time*d-*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_DATE_TIME_d)
execute_process(COMMAND cmd /c "dir *_filesystem*d-*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_FILESYSTEM_d)
execute_process(COMMAND cmd /c "dir *_system*d-*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_SYSTEM_d)
execute_process(COMMAND cmd /c "dir *_thread*d-*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_THREAD_d)

execute_process(COMMAND cmd /c "dir *_chrono*mt-x*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_CHRONO)
execute_process(COMMAND cmd /c "dir *_date_time*mt-x*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_DATE_TIME)
execute_process(COMMAND cmd /c "dir *_filesystem*mt-x*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_FILESYSTEM)
execute_process(COMMAND cmd /c "dir *_system*mt-x*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_SYSTEM)
execute_process(COMMAND cmd /c "dir *_thread*mt-x*.* /B" WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/lib/" OUTPUT_VARIABLE BOOST_THREAD)



set(SOURCE_FILES
	db/builder.cc
    db/c.cc
    db/dbformat.cc
    db/db_bench.cc
    db/db_impl.cc
    db/db_iter.cc
    db/filename.cc
    db/log_reader.cc
    db/log_writer.cc
    db/memtable.cc
    db/repair.cc
    db/table_cache.cc
    db/version_edit.cc
    db/version_set.cc
    db/write_batch.cc
    port/port_win.cc
    table/block.cc
    table/block_builder.cc
    table/format.cc
    table/iterator.cc
    table/merger.cc
    table/table.cc
    table/table_builder.cc
    table/two_level_iterator.cc
    util/arena.cc
    util/cache.cc
    util/coding.cc
    util/comparator.cc
    util/crc32c.cc
    util/env.cc
    util/env_boost.cc
    util/hash.cc
    util/histogram.cc
    util/logging.cc
    util/options.cc
    util/status.cc
    util/testharness.cc
    util/testutil.cc
    util/win_logger.cc
)
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)
ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")



add_library(leveldb_wbd STATIC ${SOURCE_FILES} ${BOOST_LIBS})



set_target_properties(leveldb_wbd PROPERTIES DEBUG_POSTFIX "d")

if(MSVC)
	if(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013")
		set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/vcvarsall.bat")
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 12 2013 Win64")
		set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/vcvarsall.bat")
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015")
		set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/vcvarsall.bat")
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 14 2015 Win64")
		set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/vcvarsall.bat")	
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017")
		set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat")
	elseif(${CMAKE_GENERATOR} STREQUAL "Visual Studio 15 2017 Win64")
		set(vcvarsall "C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/VC/Auxiliary/Build/vcvarsall.bat")	
	endif()
	set(DEBUG_COMMAND lib.exe\ /OUT:"$(OutDir)leveldbd.lib"\ "$(OutDir)leveldb_wbdd.lib"\ "$(SolutionDir)..\\lib\\${BOOST_CHRONO_d}"\ "$(SolutionDir)..\\lib\\${BOOST_DATE_TIME_d}"\ "$(SolutionDir)..\\lib\\${BOOST_FILESYSTEM_d}"\ "$(SolutionDir)..\\lib\\${BOOST_SYSTEM_d}"\ "$(SolutionDir)..\\lib\\${BOOST_THREAD_d}")
	string(REGEX REPLACE "\n" "" DEBUG_COMMAND ${DEBUG_COMMAND})
	set(RELEASE_COMMAND lib.exe\ /OUT:"$(OutDir)leveldb.lib"\ "$(OutDir)leveldb_wbd.lib"\ "$(SolutionDir)..\\lib\\${BOOST_CHRONO}"\ "$(SolutionDir)..\\lib\\${BOOST_DATE_TIME}"\ "$(SolutionDir)..\\lib\\${BOOST_FILESYSTEM}"\ "$(SolutionDir)..\\lib\\${BOOST_SYSTEM}"\ "$(SolutionDir)..\\lib\\${BOOST_THREAD}")
	string(REGEX REPLACE "\n" "" RELEASE_COMMAND ${RELEASE_COMMAND})
	message(${DEBUG_COMMAND})
	message(${RELEASE_COMMAND})
	add_custom_command(TARGET leveldb_wbd
                   POST_BUILD
                   COMMAND ${vcvarsall} && if "$(Configuration)" == Debug (${DEBUG_COMMAND})
				   COMMAND ${vcvarsall} && if "$(Configuration)" == Release (${RELEASE_COMMAND})
	)
endif(MSVC)

if(${CMAKE_GENERATOR} STREQUAL "MinGW Makefiles")
	string(REPLACE "bin/g++.exe" "x86_64-w64-mingw32/lib/libmsvcr120.a" LIBMSVCR120 ${CMAKE_CXX_COMPILER})
	
	string(REGEX REPLACE "/" "\\\\" WINDIR ${PROJECT_SOURCE_DIR})
	string(REGEX REPLACE "/" "\\\\" LIBMSVCR120 ${LIBMSVCR120})
	
	execute_process(COMMAND cmd /c copy "${WINDIR}\\lib\\${BOOST_CHRONO}" .)
	execute_process(COMMAND cmd /c copy "${WINDIR}\\lib\\${BOOST_DATE_TIME}" .)
	execute_process(COMMAND cmd /c copy "${WINDIR}\\lib\\${BOOST_FILESYSTEM}" .)
	execute_process(COMMAND cmd /c copy "${WINDIR}\\lib\\${BOOST_SYSTEM}" .)
	execute_process(COMMAND cmd /c copy "${WINDIR}\\lib\\${BOOST_THREAD}" .)
	execute_process(COMMAND cmd /c copy "${LIBMSVCR120}" .)
	
	
	add_custom_command(TARGET leveldb_wbd
                   POST_BUILD
                   COMMAND ar -x ${BOOST_CHRONO}
				   COMMAND ar -x ${BOOST_DATE_TIME}
				   COMMAND ar -x ${BOOST_FILESYSTEM}
				   COMMAND ar -x ${BOOST_SYSTEM}
				   COMMAND ar -x ${BOOST_THREAD}
				   COMMAND ar -x libleveldb_wbd.a
				   COMMAND ar -x libmsvcr120.a
				   COMMAND DEL libleveldb.a 2>&1 >NUL
				   COMMAND ar -qc libleveldb.a *.obj
				   COMMAND ar -qc libleveldb.a *.o
				   COMMAND DEL *.o
				   COMMAND DEL *.obj
	)
endif()

#78,78,156
# g++ main.cpp -std=c++11 -lboost_chrono-mgw81-mt-x64-1_68 -lleveldb_wbd -lboost_date_time-mgw81-mt-x64-1_68 -lboost_filesystem-mgw81-mt-x64-1_68 -lboost_system-mgw81-mt-x64-1_68 -lboost_thread-mgw81-mt-x64-1_68
