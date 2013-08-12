#define MAX_BYTE 256
#define MAX_CHARS MAX_BYTE
#define MAX_INDEX MAX_BYTE
#define AC_ID 0x05
#define echo_error(_x)       {std::cout << _x << std::endl;}

#define echo_msg_arg_error(_x)    {std::cout << "Incorrect number of arguments in" <<_x << std::endl;}
#define echo_msg_name_error(_x)   {std::cout << "expected msg "<<_x << " not received" << std::endl; }

#define echo_msg_conversion_error(_x,_y) {std::cout << "could not convert "<<_x <<" to " << _y;}

#define echo_data_type_error(_x)  {std::cout<< "type with name "<< _x << " has not been implemented yet"<<std::endl;}
