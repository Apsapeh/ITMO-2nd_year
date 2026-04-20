TITLE 'K'

VARIABLES        
u

DEFINITIONS
k = 450              
h = 25              
u_env = 20          

{ Центр масс }
area = SURF_INTEGRAL(1) 
Xc = SURF_INTEGRAL(x) / area
Yc = SURF_INTEGRAL(y) / area

{ Лазер }
q0 = 50000          
R = 0.3             
Q = q0 * exp(-((x - Xc)^2 + (y - Yc)^2) / R^2)

EQUATIONS
div(k*grad(u)) + Q = 0

BOUNDARIES
REGION 1
START(0,0)
VALUE(u) = 200                  LINE TO (1,0)        
NATURAL(u) = -h*(u - u_env)     LINE TO (1, 1.5)     
NATURAL(u) = -h*(u - u_env)     LINE TO (2, 0)       
VALUE(u) = 200                  LINE TO (3, 0)       
NATURAL(u) = -h*(u - u_env)     LINE TO (1.5, 2)     
NATURAL(u) = -h*(u - u_env)     LINE TO (3, 4)       
NATURAL(u) = -h*(u - u_env)     LINE TO (2, 4)       
NATURAL(u) = -h*(u - u_env)     LINE TO (1, 2.5)     
NATURAL(u) = -h*(u - u_env)     LINE TO (1, 4)       
NATURAL(u) = -h*(u - u_env)     LINE TO (0, 4)       
NATURAL(u) = -h*(u - u_env)     LINE TO CLOSE        

PLOTS
CONTOUR(u)
SURFACE(u)
END