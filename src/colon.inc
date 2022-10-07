%define dict_pointer 0

%macro colon 2  ;   colon (key, label)

    %%next: 
    dq dict_pointer ;   address to next pair
    db %1, 0        ;   key
    %2:             ;   value
        
    %define dict_pointer %%next ;   now dict_pointer refers to this element
%endmacro

