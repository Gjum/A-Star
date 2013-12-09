A-Star
======

Basic implementation of the A-Star algorithm written in dirty C++.

A-Star is a method to find the shortest path between two points.

The calculation is slowed down for you to follow the creation of the path.

Possible output:

    ######################
    #                    #
    #                  F #
    #OOOOOOOOOOOOOOOOOOvO#
    #v<<<<<<<<<<<<<<<<<<O#
    #v#########OOOOOOOOO #
    #>vvvvvvvv#          #
    #Ovvvvvvvv#          #
    #Ovvvvvvvv#          #
    #OS<<<<<<<#          #
    # OOOO^^^^#          #
    ######################

    S = start
    F = finish
    # = wall
    O = open for checking
    <>^v = when going from finish to start, follow the arrows
    (you can always swap F and S to find a way from the start to the finish)

Don't expect too much from a one-day project! ;)

