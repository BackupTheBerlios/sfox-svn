!!ARBfp1.0

TEMP c, c2, tcoord, N, d, sum;
#PARAM dx = { 0.001953125, 0, 0, 0};
#PARAM dy = { 0, 0.001953125, 0, 0};
PARAM dx = { .00390625, 0, 0, 0};
PARAM dy = { 0, .00390625, 0, 0};
PARAM one = { 0.5, 0.5, 0.5, 0.5};

ADD tcoord, fragment.texcoord[0], -dx;
TEX c, fragment.texcoord[0], texture[0], 2D;
ADD tcoord, fragment.texcoord[0], dx;
TEX c2, tcoord, texture[0], 2D;
SUB N.x, c.w, c2.w;

ADD tcoord, fragment.texcoord[0], -dy;
TEX c, fragment.texcoord[0], texture[0], 2D;
ADD tcoord, fragment.texcoord[0], dy;
TEX c2, tcoord, texture[0], 2D;
SUB N.y, c.w, c2.w;

MOV N.z, 1.0;

DP3 N.w, N, N;
RSQ N.w, N.w;
MUL N, N, N.w;

MAD N, N, one.w, one;

MOV result.color.x, N.x;
MOV result.color.y, N.y;
MOV result.color.z, N.z;
END
