cmake_minimum_required(VERSION 3.16)
project(src C)

set(CMAKE_C_STANDARD 11)

add_executable(01-client hello-client.c)

add_executable(01-server hello-server.c)
add_executable(low_open low_open.c)
add_executable(low_read low_read.c)
add_executable(fd_seri fd_seri.c)
add_executable(endian_conv endian_conv.c)
add_executable(inet_addr inet_addr.c)
add_executable(inet_aton inet_aton.c)
add_executable(inet_ntoa inet_ntoa.c)

add_executable(echo_server echo_server.c)
add_executable(echo_client echo_client.c)
add_executable(echo_client2 echo_client2.c)

add_executable(op_client op_client.c)
add_executable(op_server op_server.c)

add_executable(op_server2 op_server2.c)
add_executable(op_client2 op_client2.c)

add_executable(uecho_server uecho_server.c)
add_executable(uecho_client uecho_client.c)
add_executable(uecho_con_client uecho_con_client.c)

add_executable(file_server file_server.c)
add_executable(file_client file_client.c)

add_executable(gethostbyname gethostbyname.c)
add_executable(gethostbyaddr gethostbyaddr.c)


add_executable(sock_type sock_type.c)

add_executable(get_buf get_buf.c)

add_executable(set_buf set_buf.c)

add_executable(reuseadr_eserver reuseadr_eserver.c)

add_executable(fork fork.c)

add_executable(zombie zombie.c)


add_executable(wait wait.c)

add_executable(waitpid waitpid.c)

add_executable(signal signal.c)

add_executable(sigaction sigaction.c)

add_executable(remove_zombie remove_zombie.c)

add_executable(echo_mpserv echo_mpserv.c)

add_executable(echo_mpclient echo_mpclient.c)

add_executable(pipe1 pipe1.c)

add_executable(pipe2 pipe2.c)

add_executable(pipe3 pipe3.c)

add_executable(echo_storeserv echo_storeserv.c)

add_executable(select select.c)

add_executable(echo_selectserv echo_selectserv.c)

add_executable(oob_send oob_send.c)
add_executable(oob_recv oob_recv.c)

add_executable(peek_send peek_send.c)
add_executable(peek_recv peek_recv.c)

add_executable(writev writev.c)

add_executable(readv readv.c)

add_executable(news_sender news_sender.c)
add_executable(news_receiver news_receiver.c)

add_executable(news_receiver_brd news_receiver_brd.c)
add_executable(news_sender_brd news_sender_brd.c)

add_executable(syscpy syscpy.c)
add_executable(stdcpy stdcpy.c)

add_executable(desto desto.c)

add_executable(todes todes.c)

add_executable(echo_stdserv echo_stdserv.c)
add_executable(echo_stdclient echo_stdclient.c)

add_executable(echo_epollserv echo_epollserv.c)

add_executable(echo_EPLTserv echo_EPLTserv.c)
add_executable(echo_EPETserv echo_EPETserv.c)

find_package(Threads REQUIRED)
add_executable(thread1 thread1.c)
target_link_libraries(thread1 Threads::Threads)

add_executable(thread2 thread2.c)
target_link_libraries(thread2 Threads::Threads)

add_executable(thread3 thread3.c)
target_link_libraries(thread3 Threads::Threads)

add_executable(thread4 thread4.c)
target_link_libraries(thread4 Threads::Threads)


add_executable(mutex mutex.c)
target_link_libraries(mutex Threads::Threads)

add_executable(semaphore semaphore.c)
target_link_libraries(semaphore Threads::Threads)

add_executable(chat_client chat_client.c)
target_link_libraries(chat_client Threads::Threads)
add_executable(chat_server chat_server.c)
target_link_libraries(chat_server Threads::Threads)

add_executable(webserv_llinux webserv_llinux.c)
target_link_libraries(webserv_llinux Threads::Threads)

