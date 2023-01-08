brainfuck calculator attempt

setup cells with char to int mapping
one value cell and one empty cell to reverse counting

[ 0 - 9 -> 48 - 57]
[ + -> 43 ]
[ - -> 45 ]

c0 is 0
>>>>

c4 is 1
+>>+>>

c8 is 2
++>>++>>

c12 is 3
+++>>+++>>

c16 is 4
++++>>++++>>

c20 is 5
+++++>>+++++>>

c24 is 6
++++++>>++++++>>

c28 is 7
+++++++>>+++++++>>

c32 is 8
++++++++>>++++++++>>

c36 is 9
+++++++++>>+++++++++>>

add 48 to every cell
++++++++
[
    >++++++
    [
        <
        <<<<+ add 48 to 9
        <<<<+ add 48 to 8
        <<<<+ 7
        <<<<+ 6
        <<<<+ 5
        <<<<+ 4
        <<<<+ 3
        <<<<+ 2
        <<<<+ 1
        <<<<+ 0
        >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>> >>>>
        >-
    ]
    <-
]
setup completed

space for global variables
>>>>>>>>>>
0 inVal
1 inVal2
2 inCount
3 inCount2
4 inBuffer
space for function parameters
>>>>>>>>>>
space for function selection
one cell per function if set to 1 executes function
0 null
1 read input
2 add input


programm loop: contains functions and main
+
[

    0 null function does nothing
    > to function pointer
    [

        - exit function
    ]
    

    1 read input 
    > to function pointer
    [
        go to buffer cell
        << <<<<<<<<<< <<<<<<

        get input
        ,

        convert input char val
        go to 0
        <<<< 

        - exit function
    ]


    2 add input
    > to function pointer
    [
        go to input cell
        <<< <<<<<<<<<< <<<<<<<<<<
        
        go to digit count cell
        >>
        [
            save cell in next cell
            >+
            <-
            multiply input by 10
            <<
            [
                >>>>++++++++++
                [
                    <<<<<+
                    >>>>>-
                ]
                <<<<-
            ]
            >
            [
                switch cell
                <+
                >-
            ]

        ]
        >
        [
            switch cell
            <+
            >-
        ]

        go to cell with input value
        >
        add input digit
        [
            <<<<
            +
            >>>>
            -
        ]

        go to this function pointer >>>>> >>>>>>>>>> >>>

        - exit function
    ]
    

    >+ 
    3 main while true
    [
        >
        get numer 1 from user
        [


        ]

        get operator from user
        [

        ]

        get numer 2 from user
        [

        ]

        get = from user
        [

        ]

        output result
        [

        ]



        <
    ]
]