 # i f n d e f   Q U E S T I O N C O N T R O L L E R _ H 
 # d e f i n e   Q U E S T I O N C O N T R O L L E R _ H 
 
 # i n c l u d e   < v e c t o r > 
 # i n c l u d e   " m i d d l e / Q u e s t i o n D a t a . h " 
 
 c l a s s   Q u e s t i o n C o n t r o l l e r 
 { 
 p u b l i c : 
         Q u e s t i o n C o n t r o l l e r ( )   { } 
         / * * 
           *   @ b r i e f  mūR ī 
           *   @ p a r a m   d a t a                     [ i n ] 
           *   @ p a r a m   q u e s t i o n _ i d       [ o u t ] 
           *   @ r e t u r n     v o i j j j d 
           *   @ a u t h o r                     l i j i a n c o n g ( l i j i a n c o n g @ g b c o m . c o m . c n ) 
           *   @ d a t e                         2 0 2 2 - 0 4 - 0 4   1 7 : 5 2 : 3 9 
           * / 
         v o i d   a d d _ q u e s t i o n ( c o n s t   Q u e s t i o n D a t a &   d a t a ,   i n t &   q u e s t i o n _ i d ) 
         { 
                 q u e s t i o n s _ [ + + q u e s t i o n _ i d _ ]   =   d a t a ; 
                 q u e s t i o n _ i d   =   q u e s t i o n _ i d _ ; 
                 r e t u r n   t r u e ; 
         } 
 
 p r i v a t e : 
         s t d : : m a p < i n t ,   Q u e s t i o n D a t a >   q u e s t i o n s _ ;     / / / <  īv   i d  T  Q[¹ 
         i n t   q u e s t i o n _ i d _   =   0 ; 
 } ; 
 
 # e n d i f   / /   Q U E S T I O N C O N T R O L L E R _ H 
