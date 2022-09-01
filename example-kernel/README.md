```c
int main(void)
{
    system_init()
    //...
    run_application();
    //...
    vTaskStartScheduler();
    while (1);

}
```
