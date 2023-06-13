# PowerStation

 Моё расчетно-графическое задание по Операционным системам из ВУЗа, пусть лежит на гитхабе, может кому-нибудь понадобится.
 
 ## Задание:
##### ГЭС имеет 4 гидрогенератора и снабжает электроэнергией 3 объекта. Мощность, потребляемая каждым объектом, меняется в течение суток относительно некоторого среднего значения по синусоидальному закону. Мощности гидрогенераторов фиксированы:  
   - Ргг1 = 100 МВт  
   - Ргг2 = 100 МВт  
   - Ргг3 = 120 МВт  
   - Ргг4 = 120 МВт  
##### Мощность, потребляемая объектами (МВт):  
   - Р1= 100 + 20*Sin(pi/2*(h/6 - 1))  
   - Р2= 180 + 50*Sin(pi/2*(h/6 - 1))  
   - Р3=90+ 20*Sin(pi/2*(h/6 - 1))  
  Где h – текущий час в течение суток (0 - 23).  
##### Написать программу, моделирующую работу энергосистемы. Временной масштаб моделирования 1 : 3600 (1 Сек соответствует 1 часу). Моделирование потребления электроэнергии каждым объектом выполняется в отдельном потоке. Первичный поток процесса получает данные от каждого объекта и при необходимости отключает (или включает) гидрогенератор 4, в зависимости от того, хватает ли суммарной мощности первых трех генераторов для энергоснабжения объектов. 
##### На главной форме визуально должны отображаться текущие мощности потребления всех объектов, а также состояние всех генераторов (включен -отключен).
