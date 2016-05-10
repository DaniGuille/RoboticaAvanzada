```
```
#
``` hexapodcontroller
```
Intro to component here

Nomenclatura de las patas
delantera izquierda(1)
delantera derecha(2)
medio izquierda(3)
medio derecha(4)
trasera izquierda(5)
trasera derecha(6) 


El componente controla los distintos tipos de movimiento con los que el hexápodo puede desplazarse.

El primero de ellos, One by One Gait, mueve de una en una las patas siguiendo el orden 1-2-3-4-5-6
El segundo, Tripod Gait, mueve las patas de tres en tres siguiendo el orden (1,4,5)-(2,3,6).
El tercero, quadruped gait, mueve las patas por parejas siguiendo el orden (1,2)-(3,4)-(5,6).

Mediante la interfaz gráfica puede seleccionarse el modo de movimiento, el cual se refleja en la simulacion del hexapodo.





## Configuration parameters
As any other component,
``` *hexapodcontroller* ```
needs a configuration file to start. In

    etc/config

you can find an example of a configuration file. We can find there the following lines:

    EXAMPLE HERE

    
## Starting the component
To avoid changing the *config* file in the repository, we can copy it to the component's home directory, so changes will remain untouched by future git pulls:

    cd

``` <hexapodcontroller 's path> ```

    cp etc/config config
    
After editing the new config file we can run the component:

    bin/

```hexapodcontroller ```

    --Ice.Config=config
