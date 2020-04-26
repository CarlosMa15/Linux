spreadsheetServer: server.o session_dispatcher.o spreadsheet_state.o client.o handlers.o message_handler.o Message.o threading.o
	g++ -o spreadsheet_server -g server.o session_dispatcher.o spreadsheet_state.o client.o handlers.o message_handler.o Message.o threading.o -pthread 

server.o: server.cpp threading.h Message.h client.h spreadsheet_state.h Message.h session_dispatcher.h
	g++ -c -g server.cpp

session_dispatcher.o: session_dispatcher.cpp threading.h server.h client.h Message.h spreadsheet_state.h message_handler.h handlers.h
	g++ -c -g session_dispatcher.cpp

Message.o: Message.cpp client.h
	g++ -c -g Message.cpp

message_handler.o: message_handler.cpp Message.h
	g++ -c -g message_handler.cpp

handlers.o: handlers.cpp client.h message_handler.h
	g++ -c -g handlers.cpp

client.o: client.cpp 
	g++ -c -g client.cpp

threading.o: threading.cpp 
	g++ -c -g threading.cpp 

spreadsheet_state.o: spreadsheet_state.cpp
	g++ -c -g spreadsheet_state.cpp

clean:
	rm spreadsheet_server *.o
