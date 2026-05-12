TITLE '9-10'

VARIABLES
    u

DEFINITIONS
    k = 3
    A = 15
    B = 1
    L = 9
    H = 7
    
    ls_y = B + (A/6.0) * (y * (6/H))

EQUATIONS
    div(k*grad(u)) = 0 

BOUNDARIES
    REGION 1
        START(0,0)

        {Bottom edge}
        VALUE(u) = (x/L)*B             LINE TO (L,0)

        {Right edge}
        VALUE(u) = ls_y                LINE TO (L,H)

        {Top edge}
        VALUE(u) = (x/L)*A             LINE TO (0,H)

        {Left edge}
        VALUE(u) = 0                   LINE TO CLOSE

PLOTS
    CONTOUR(u)
END