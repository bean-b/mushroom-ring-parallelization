#include <stdio.h>
#include "gnuplot_i.h"

void drawFirstAndLast() {
    // gnuplot_ctrl * plt;
        
    // plt = gnuplot_init();
    // gnuplot_cmd(plt,"set terminal x11 title 'initial field'");
    // gnuplot_cmd(plt, "set cbrange [0:9]");
    // gnuplot_cmd(plt, "set xrange [0:*]");
    // gnuplot_cmd(plt, "set yrange [0:*]");
    // gnuplot_cmd(plt, " set palette defined (\
    //     0 '#b9f0c2', \
    //     1 '#84b2db', \
    //     2 '#629acc', \
    //     3 '#498ecc', \
    //     4 '#7749cc', \
    //     5 '#6d3ec2', \
    //     6 '#a080d9', \
    //     7 '#bfade0', \
    //     8 '#c0b4d6', \
    //     9 '#d9d1b6' \
    // )");
    // //0 empty
    // //1 spore
    // //2 young
    // //3 maturing
    // //4 mushrooms
    // //5 older
    // //6 decaying
    // //7 dead1
    // //8 dead2
    // //9 inert
    // gnuplot_cmd(plt, "set title 'Initial field'");
    // gnuplot_cmd(plt, " plot 'initial.dat' matrix with image");

    gnuplot_ctrl * plt2;
        
    plt2 = gnuplot_init();
    gnuplot_cmd(plt2,"set terminal x11 title 'final field'");
    gnuplot_cmd(plt2, "set cbrange [0:9]");
    gnuplot_cmd(plt2, "set xrange [0:*]");
    gnuplot_cmd(plt2, "set yrange [0:*]");
    gnuplot_cmd(plt2, " set palette defined (\
        0 '#b9f0c2', \
        1 '#84b2db', \
        2 '#629acc', \
        3 '#498ecc', \
        4 '#7749cc', \
        5 '#6d3ec2', \
        6 '#a080d9', \
        7 '#bfade0', \
        8 '#c0b4d6', \
        9 '#d9d1b6' \
    )");
    //0 empty
    //1 spore
    //2 young
    //3 maturing
    //4 mushrooms
    //5 older
    //6 decaying
    //7 dead1
    //8 dead2
    //9 inert
    gnuplot_cmd(plt2, "set title 'Final field'");
    gnuplot_cmd(plt2, " plot 'final.dat' matrix with image");


    printf("Press Enter to exit...\n");
    getchar();
    
}
