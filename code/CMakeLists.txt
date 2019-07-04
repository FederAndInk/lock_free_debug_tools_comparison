cmake_minimum_required(VERSION 3.10)
project(lock_free_problems VERSION 0.1.0)

find_package(Boost REQUIRED
             COMPONENTS atomic thread)

set(USE_LIBCXX OFF CACHE BOOL "using libc++")
set(OPTIMIZATION_LEVEL 0
    CACHE STRING "optimization")
set_property(CACHE OPTIMIZATION_LEVEL
             PROPERTY STRINGS
                      0
                      1
                      2
                      3)

message("USE_LIBCXX: " ${USE_LIBCXX})
message("OPTIMIZATION_LEVEL: "
        ${OPTIMIZATION_LEVEL})

add_compile_options(-pedantic
                    -Wall
                    -Wextra
                    -Wconversion
                    -Wold-style-cast
                    -Werror=return-type)

function(add_exe tname)
  add_executable(${tname} ${ARGN})
  target_link_libraries(${tname}
                        PRIVATE pthread
                                atomic
                                Boost::boost
                                Boost::thread
                                Boost::atomic)
  target_compile_features(${tname} PRIVATE
                          cxx_std_17)
  target_compile_options(
    ${tname}
    PRIVATE
      -O${OPTIMIZATION_LEVEL} -g
      $<$<BOOL:${USE_LIBCXX}>:-stdlib=libc++>)
  target_link_options(
    ${tname} PRIVATE
    $<$<BOOL:${USE_LIBCXX}>:-stdlib=libc++>)
endfunction(add_exe)

function(add_exe_san tname san)
  add_exe(${tname}-${san}san ${ARGN})
  target_compile_options(
    ${tname}-${san}san
    PRIVATE -fsanitize=${san})
  target_link_options(${tname}-${san}san PRIVATE
                      -fsanitize=${san})
endfunction(add_exe_san)

function(add_exe_sans tname)
  add_exe(${tname} ${ARGN})
  add_exe(${tname}-valgrind ${ARGN})
  target_compile_definitions(${tname}-valgrind
                             PRIVATE USE_VALGRIND)

  add_exe_san(${tname} thread ${ARGN})
  add_exe_san(${tname} address ${ARGN})
  if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    add_exe_san(${tname} memory ${ARGN})
  endif("${CMAKE_CXX_COMPILER_ID}" MATCHES
        "Clang")
  add_exe_san(${tname} undefined ${ARGN})
endfunction(add_exe_sans)

add_exe_sans(data_race_simple
             ./data_race/data_race_simple.cpp)
add_exe_sans(data_race_string
             ./data_race/data_race_string.cpp)
add_executable(data_race_relacy
               ./data_race/data_race_relacy.cpp)
target_link_libraries(data_race_relacy
                      PRIVATE pthread)
target_include_directories(
  data_race_relacy
  PRIVATE ${CMAKE_CURRENT_LIST_DIR}/relacy/)
target_compile_options(data_race_relacy
                       PRIVATE -std=c++03)

add_exe_sans(race_map ./data_race/race_map.cpp)
add_exe_sans(data_race_race_cond
             ./data_race/data_race_race_cond.cpp)
add_exe_sans(bit_fields
             ./data_race/bit_fields.cpp)
add_exe_sans(pseudo_notif
             ./data_race/pseudo_notif.cpp)
add_exe_sans(race_destruction
             ./data_race/race_destruction.cpp)
add_exe_sans(
  race_destruction_string
  ./data_race/race_destruction_string.cpp)
add_exe_sans(race_destruction_SSO
             ./data_race/race_destruction_SSO.cpp)

add_exe_sans(
  atomic_fix_data_race_simple
  ./atomic/atomic_fix_data_race_simple.cpp)

add_exe_sans(aba ./aba/aba.cpp)
add_exe_sans(aba_detect ./aba/aba_detect.cpp)
add_exe_sans(aba_sync ./aba/aba_sync.cpp)
add_exe_sans(heap_reuse ./aba/heap_reuse.cpp)

add_exe_sans(aba_fixed ./aba/aba_fixed.cpp)

add_exe_sans(notif_na ./prod_cons/notif_na.cpp)
add_exe_sans(notif_acq_rel
             ./prod_cons/notif_acq_rel.cpp)
add_exe_sans(notif_seq_cst
             ./prod_cons/notif_seq_cst.cpp)
add_exe_sans(notif_relaxed
             ./prod_cons/notif_relaxed.cpp)
add_exe_sans(notif_wrong_acq_rel
             ./prod_cons/notif_wrong_acq_rel.cpp)
add_exe_sans(
  notif_wrong_acq_rel_2
  ./prod_cons/notif_wrong_acq_rel_2.cpp)

add_exe_sans(
  store_load_acq_rel_sem
  ./memory_ordering/store_load_acq_rel_sem.cpp)
add_exe_sans(
  store_load_relaxed
  ./memory_ordering/store_load_relaxed.cpp)
add_exe_sans(
  store_load_seq_cst
  ./memory_ordering/store_load_seq_cst.cpp)

add_exe_sans(boost_queue ./boost/boost_queue.cpp)
