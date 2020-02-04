
#include"g_callfunction.h"

GtkWidget *searchFrame, *inputBox, *meaningBox, *navigation, *addButton, *editButton, *infoButton, *deleteButton, *searchButton, *scroll;
GtkWidget *window;
GtkWidget *frameBody;
GtkWidget *textView;
GtkWidget *textSearch;
BTA *data;
GtkTextBuffer *buffer;


void creatSoundexTree(BTA **soundexTree){
	char en[40],vi[100000];
	int rsize,i=0;
	char soundex[5];
	*soundexTree=btcrt("soundexTree",0,0);
	
	btpos(data, 1);
	while(btseln(data,en,vi,100000*sizeof(char),&rsize)==0){
		SoundEx(soundex,en,4,1);
		btins(*soundexTree,en,soundex,5*sizeof(char));
		i++;
	}
	g_print("da them %d vao soundexTree\n",i);
}


void main(int argv, char **argc){
	BTA *soundexTree;
	btinit();
	data = btopn("AnhViet.dat",0,1);
	if(data == NULL){
		FILE* datafile;
		if ((datafile = fopen("AnhViet.txt", "r")) == NULL) {
    		printf("Loi mo file\n");
   			exit(0);
  		}
	  	int line = 0;
	  	BTA *convertfile;
	  	convertfile = btcrt("AnhViet.dat", 0, 0);
	  	char name[100], mean[10000];
	  	while (fscanf(datafile, "%[^\t]", name) == 1) {
	    	fgets(mean, 10000, datafile);
	     	separate_mean(mean);
			line =line+1;
	     	btins(convertfile, name, mean, strlen(mean) + 1);
	  	}
	  	printf("Da doc %d dong .\n",line);
	  	btcls(convertfile);
	  	fclose(datafile);
	  	data = btopn("AnhViet.dat",0,1);
	}
	soundexTree = btopn("soundexTree", 0, 0);
    if(!soundexTree){
		printf("Creating SoundexTree...\n");
		creatSoundexTree(&soundexTree);
	}
	gtk_init(&argv, &argc);

	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_size_request(window, 650, 500);
	gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	gtk_window_set_title(GTK_WINDOW(window), "Englist-Vietnamese Dictionary");
	g_signal_connect(window, "delete_event", gtk_main_quit, NULL);
	gtk_window_set_resizable(GTK_WINDOW(window), FALSE);


	frameBody = gtk_fixed_new();
	gtk_container_add(GTK_CONTAINER(window), frameBody);
	gtk_widget_set_margin_start(frameBody, 5);
	gtk_widget_set_margin_end(frameBody, 5);
	gtk_widget_set_margin_top(frameBody, 5);
	gtk_widget_set_margin_bottom(frameBody, 15);

	navigation = gtk_toolbar_new();
	gtk_toolbar_set_style(GTK_TOOLBAR(navigation), GTK_TOOLBAR_BOTH);
	gtk_widget_set_size_request(navigation, 530, 50);
	gtk_fixed_put(GTK_FIXED(frameBody), navigation, 0, 0);
	


	addButton = gtk_tool_button_new(gtk_image_new_from_icon_name(GTK_STOCK_ADD, GTK_ICON_SIZE_SMALL_TOOLBAR), "Add");
	editButton = gtk_tool_button_new(gtk_image_new_from_icon_name(GTK_STOCK_EDIT, GTK_ICON_SIZE_SMALL_TOOLBAR), "Edit");
	deleteButton = gtk_tool_button_new(gtk_image_new_from_icon_name(GTK_STOCK_DELETE, GTK_ICON_SIZE_SMALL_TOOLBAR), "Delete");
	infoButton = gtk_tool_button_new(gtk_image_new_from_icon_name(GTK_STOCK_DIALOG_INFO, GTK_ICON_SIZE_SMALL_TOOLBAR), "About Us");
	
	gtk_toolbar_insert(GTK_TOOLBAR(navigation), addButton, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(navigation), editButton, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(navigation), deleteButton, -1);
	gtk_toolbar_insert(GTK_TOOLBAR(navigation), infoButton, -1);

	searchFrame = gtk_frame_new("Search Box");
	gtk_widget_set_size_request(searchFrame, 640, 65);
	gtk_fixed_put(GTK_FIXED(frameBody), searchFrame, 5, 65);
	inputBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);	
	
	textSearch = gtk_search_entry_new();
	gtk_widget_set_size_request(textSearch, 550, 40);
	searchButton = gtk_button_new_with_label("Search");
	gtk_widget_set_margin_bottom(searchButton, 10);
	gtk_widget_set_margin_top(searchButton, 10);
	gtk_box_pack_start(GTK_BOX(inputBox),textSearch, 1, 1, 0);
	gtk_box_pack_start(GTK_BOX(inputBox),searchButton, 1, 1, 0);
	gtk_container_add(GTK_CONTAINER(searchFrame), inputBox);



	meaningBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_widget_set_size_request(meaningBox, 635, 500);
	gtk_fixed_put(GTK_FIXED(frameBody), meaningBox, 5, 140);
	textView = gtk_text_view_new();
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(textView), GTK_WRAP_WORD_CHAR);
	scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add(GTK_CONTAINER(scroll), textView);
	gtk_box_pack_start(GTK_BOX(meaningBox), scroll, 1, 1, 0);
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textView));
	gtk_text_buffer_set_text(buffer, "Hello World!!", -1);

	gtk_widget_show_all(window);


	g_signal_connect(infoButton, "clicked", G_CALLBACK(show_about_us_dialog), NULL);
	g_signal_connect(searchButton, "clicked", G_CALLBACK(display_meaning), soundexTree);
	g_signal_connect(textSearch, "activate", G_CALLBACK(display_meaning), soundexTree);
	g_signal_connect(textSearch, "search-changed", G_CALLBACK(suggest_function), soundexTree);
	g_signal_connect(textSearch, "focus", G_CALLBACK(auto_complete), soundexTree);
	g_signal_connect(addButton, "clicked", G_CALLBACK(show_add_dialog), soundexTree);
	g_signal_connect(editButton, "clicked", G_CALLBACK(show_edit_dialog), NULL);
	g_signal_connect(deleteButton, "clicked", G_CALLBACK(show_delete_dialog), soundexTree);
	
	gtk_main();
	btcls(soundexTree);
	btcls(data);
}
