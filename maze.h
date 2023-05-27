#pragma once // тоже самое, что и #ifndef + #define. Это директива препроцессора, которая гарантирует, что файл заголовка будет включен только один раз в процессе компиляции
#include <iostream>
#include <memory>
#include <random>
#include <vector>

class Maze {
 public:
/*
	Этот метод генерирует лабиринт заданной ширины и высоты. Если ширина или высота меньше единицы, то метод вернет nullptr. 
	В противном случае, метод создает двумерный вектор символов cave с помощью std::make_shared<std::vector<std::vector<char>>>. 
	Затем он вызывает следующие методы для создания пути в лабиринте: initializecave, createPaths, removeDeadEnds и connectLastRow. 
	В конце метод возвращает указатель на созданный лабиринт
*/
  static std::shared_ptr<std::vector<std::vector<char>>>  // Метод Functionality, который принимает latitude (ширину) и altitude (высоту) в качестве входных параметров. 
  Functionality(unsigned latitude, unsigned altitude) {   // Метод возвращает указатель shared_ptr на двумерный вектор символов (std::vector<std::vector<char>>)
    if ((latitude < 1) || (altitude < 1)) 	              // Эти строки проверяют, что latitude и altitude больше или равны единице 
      return nullptr;				 		                          // Если это не так, то метод возвращает nullptr, указывая на некорректные входные параметры

    const unsigned outputAltitude = altitude * 2 + 1; 	            // Эти строки вычисляют размеры результирующего лабиринта. Высота (outputAltitude) 
    const unsigned outputLatitude = latitude * 2 + 1;               // и ширина (outputLatitude) увеличиваются вдвое и добавляется единица, чтобы учесть стены между ячейками
    auto cave = std::make_shared<std::vector<std::vector<char>>>(); // Эта строка создает динамический двумерный вектор cave, 
    cave->reserve(outputAltitude);									                // используя make_shared, и резервирует память для outputAltitude строк

    std::vector<unsigned> rowSet;       	    // Эти строки создают вектор rowSet, который будет 
    rowSet.reserve(latitude);				          // использоваться для отслеживания наборов строк в 
    for (unsigned i = 0; i < latitude; ++i)   // лабиринте. Вектор резервирует память для latitude 
      rowSet.push_back(0);				 	          // элементов и заполняется значениями 0

    initializecave(cave, outputAltitude, outputLatitude);   // Эти строки вызывают различные методы, 
    createPaths(cave, latitude, altitude);							 	  // которые инициализируют лабиринт, создают 
    removeDeadEnds(cave, latitude, outputAltitude, rowSet); // пути в лабиринте, удаляют тупики и 
    connectLastRow(cave, latitude, outputAltitude, rowSet);	// соединяют последнюю строку
    return cave;                                            // Эта строка возвращает указатель на созданный лабиринт cave.
  }
/*
	Этот метод принимает указатель на лабиринт и выводит его в консоль. Если указатель равен nullptr, то метод ничего не делает. 
	В ином случае, метод проходит по элементам лабиринта и выводит их в консоль
*/
  static void															                              // Эта строка объявляет метод Output, который принимает указатель shared_ptr 
  Output(const std::shared_ptr<std::vector<std::vector<char>>> &cave) {	// на двумерный вектор символов cave в качестве входного параметра. Метод не возвращает значения.
    if (cave == nullptr)  // Эта строка проверяет, что указатель cave не является nullptr. 
      return;			        // Если cave равен nullptr, то метод завершается без выполнения дальнейших действий

    for (unsigned i = 0; i < cave->size(); ++i) {       // Эти строки выводят содержимое лабиринта на консоль. 
      for (unsigned j = 0; j < cave->at(0).size(); ++j) // Внешний цикл проходит по строкам лабиринта, а внутренний 
        std::cout << cave->at(i).at(j);				        	// цикл проходит по символам в каждой строке и выводит их на экран. 
      std::cout << std::endl;						              	// После вывода каждой строки происходит перевод строки (std::endl), 
    }												                          	// чтобы следующая строка была выведена на новой строке в консоли
  }

 private:
  Maze() = default; // объявление конструктора по умолчанию для класса cave. Ключевое слово default указывает компилятору сгенерировать конструктор по умолчанию
/*
	Этот метод инициализирует лабиринт, заполняя его символами. Он создает двумерный вектор cave с заданными высотой и шириной, 
	затем проходит по каждому элементу и устанавливает соответствующие символы в зависимости от их позиции. Символы '@' обозначают стены, 
	а символы ' ' обозначают проходы в лабиринте
*/
  static void															                              // Объявление и определение статической функции initializecave, которая принимает 
  initializecave(std::shared_ptr<std::vector<std::vector<char>>> &cave, // ссылку на указатель на std::vector<std::vector<char>> в качестве аргумента, 
                 unsigned outputAltitude, unsigned outputLatitude) {	  // а также два беззнаковых целых числа outputAltitude и outputLatitude
    for (unsigned i = 0; i < outputAltitude; ++i) { // Начало цикла, который будет выполняться для каждой строки лабиринта. Переменная i инициализируется 
	// нулевым значением и будет увеличиваться на единицу на каждой итерации до тех пор, пока i не станет больше или равно outputAltitude

      std::vector<char> row;	 					            // Создание вектора row, который будет представлять текущую строку 
      row.reserve(outputLatitude); 				          // лабиринта. Выделение памяти для outputLatitude элементов вектора row
      for (unsigned j = 0; j < outputLatitude; ++j) // Начало цикла, который будет выполняться для каждого столбца лабиринта в текущей строке. 
	  // Переменная j инициализируется нулевым значением и будет увеличиваться на единицу на каждой итерации до тех пор, пока j не станет больше или равно outputLatitude

        if ((i % 2 == 1) && (j % 2 == 1))                       // Если текущие индексы i и j являются нечетными числами, добавляем символ 
          row.push_back(' ');			                              // пробела в текущую строку row. Это представляет проход между клетками лабиринта
        else if (((i % 2 == 1) && (j % 2 == 0) && (j != 0) &&   // Иначе, если текущие индексы i и 
                  (j != outputLatitude - 1)) ||				          // j удовлетворяют определенным условиям, 
                 ((j % 2 == 1) && (i % 2 == 0) && (i != 0) &&	  // добавляем символ дефиса в текущую 
                  (i != outputAltitude - 1)))					          // строку row. Это представляет 
          row.push_back(' ');								                  	// стены между клетками лабиринта.
        else				                   // В противном случае добавляем символ @ в текущую строку row. 
          row.push_back('@');	         // Это представляет препятствия или недоступные области в лабиринте.
      cave->push_back(std::move(row)); // Добавляем текущую строку row в вектор cave, представляющий лабиринт. 
	  								                   // С помощью std::move мы перемещаем row в вектор cave, что позволяет избежать копирования данных
    }
  }
/*
	Этот метод создает пути в лабиринте, используя алгоритм похожий на алгоритм Эллера. Он создает вектор rowSet для отслеживания группы ячеек, 
	инициализирует его значениями 0. Затем метод использует случайные числа для принятия решений о создании стен или проходов между ячейками. 
	Он также обновляет значения вектора rowSet для объединения групп ячеек. По завершению метода, лабиринт будет содержать пути между ячейками в символах '@'
*/
  static void createPaths(std::shared_ptr<std::vector<std::vector<char>>> &cave,	// Объявление и определение статической функции createPaths, которая принимает ссылку на указатель 
                          unsigned latitude, unsigned altitude) {				        	// на std::vector<std::vector<char>> в качестве аргумента, а также два беззнаковых целых числа width и height
    std::vector<unsigned> rowSet;       	   // Создание вектора rowSet, который представляет набор 
    rowSet.reserve(latitude);				         // значений для каждого столбца лабиринта. Выделяется память 
    for (unsigned i = 0; i < latitude; ++i)  // для latitude элементов вектора rowSet. Затем вектор rowSet 
      rowSet.push_back(0);				 	         // заполняется нулевыми значениями. Переменная set 
    unsigned set = 1;					 	             // инициализируется значением 1

    std::random_device rd;                         // Создание объектов для генерации случайных чисел. std::random_device используется для получения случайного зерна
    std::mt19937 mt(rd());						             // std::mt19937 является генератором случайных чисел с использованием Mersenne Twister
    std::uniform_int_distribution<int> dist(0, 2); // std::uniform_int_distribution<int> используется для генерации случайных целых чисел в заданном диапазоне

    for (unsigned i = 0; i < altitude; ++i) {   // Начало цикла, который будет выполняться для каждой строки лабиринта. Переменная i инициализируется нулевым значением и 
	// будет увеличиваться на единицу на каждой итерации до тех пор, пока i не станет больше или равно altitude
      for (unsigned j = 0; j < latitude; ++j)   // Внутренний цикл, который присваивает каждому элементу вектора rowSet
        if (rowSet[j] == 0)				            	// уникальное значение set, если элемент равен нулю. 
          rowSet[j] = set++;				          	// Затем значение set увеличивается на единицу

      for (unsigned j = 0; j < latitude - 1; ++j) {				         // Внутренний цикл, который обрабатывает  
        const auto right_wall = dist(mt);						               // горизонтальные стены между ячейками лабиринта. Генерируется случайное число 
        if ((right_wall == 1) || (rowSet[j] == rowSet[j + 1]))     // right_wall в диапазоне от 0 до 2. Если right_wall равно 1 или значения 
          cave->at(i * 2 + 1).at(j * 2 + 2) = '@';				         // rowSet[j] и rowSet[j + 1] равны, то стена между ячейками не 
        else {												                          	 // создается и символ @ помещается в соответствующую позицию в векторе 
          const auto changing_set = rowSet[j + 1];				         // cave. В противном случае значениям rowSet[j], 
          for (unsigned l = 0; l < latitude; ++l)				           // равным rowSet[j + 1], присваивается значение 
            if (rowSet[l] == changing_set)						             // rowSet[j] для объединения соединенных ячеек
              rowSet[l] = rowSet[j];
        }
      }

      for (unsigned j = 0; j < latitude; ++j) {					           // Внутренний цикл, который обрабатывает вертикальные 
        const auto bottomWall = dist(mt);					              	 // стены между ячейками лабиринта. Генерируется 
        unsigned int countCurrentSet = 0;					              	 // случайное число bottomWall в диапазоне от 
        for (unsigned l = 0; l < latitude; ++l)		          			 // 0 до 2. Если bottomWall равно 1 и количество 
          if (rowSet[j] == rowSet[l])					                		 // ячеек с текущим значением rowSet[j] не 
            countCurrentSet++;							                    	 // равно 1, то стена между ячейками 
        if ((bottomWall == 1) && (countCurrentSet != 1))      		 // не создается и символ @ помещается 
          cave->at(i * 2 + 2).at(j * 2 + 1) = '@';				         // в соответствующую позицию в векторе cave
      }

      if (i != altitude - 1) {									                   // Условие проверяет, если текущая строка не является последней 
        for (unsigned j = 0; j < latitude; ++j) {			          	 // строкой лабиринта. Если это так, то выполняется следующий блок кода:
          unsigned countHole = 0;								                   // Внутренний цикл обрабатывает ячейки текущей строки и проверяет, 
          for (unsigned l = 0; l < latitude; ++l)			          	 // есть ли отверстия (символ -) между ячейками с одинаковыми значениями 
            if ((rowSet[l] == rowSet[j]) &&				              	 // rowSet[j]. Если количество отверстий равно 0, то ставится 
                (cave->at(i * 2 + 2).at(l * 2 + 1) == ' '))	    	 // символ - в соответствующую позицию в векторе cave.
              countHole++;										                     // Затем проходится по ячейкам текущей строки и если 
          if (countHole == 0)								                    	 // в ячейке находится символ @, то значение 
            cave->at(i * 2 + 2).at(j * 2 + 1) = ' ';		        	 // rowSet[j] устанавливается равным 0, 
        }														                               // чтобы обозначить, что ячейка 
        for (unsigned j = 0; j < latitude; ++j)					           // не соединена с другими ячейками
          if (cave->at(i * 2 + 2).at(j * 2 + 1) == '@')
            rowSet[j] = 0;
      }
    }
  }
/*
	Этот метод удаляет "тупики" в лабиринте, то есть проходы, которые не ведут к выходу. Он проходит по каждой ячейке в предпоследней строке 
	лабиринта и проверяет, есть ли соединенные проходы в следующей строке. Если есть только один проход, то ячейка становится стеной
*/
  static void																	                            	// Это объявление функции removeDeadEnds, которая удаляет 
  removeDeadEnds(std::shared_ptr<std::vector<std::vector<char>>> &cave,			// тупики в лабиринте. Функция принимает указатель на 
                 unsigned latitude, unsigned outputAltitude,	              // вектор векторов символов cave, ширину и высоту лабиринта,
                 const std::vector<unsigned> &rowSet) {							        // высоту вывода и вектор rowSet
    for (unsigned int i = 0; i < latitude - 1; ++i) { 	  	// Это начало цикла, который проходит по каждому столбцу лабиринта, за исключением последнего столбца.
      if (rowSet[i] != rowSet[i + 1])					            	// Эта строка проверяет, относятся ли два соседних столбца к разным множествам (заданы в векторе rowSet). 
        cave->at(outputAltitude - 2).at(i * 2 + 2) = ' '; 	// Если это так, то тупик в этом месте лабиринта. Здесь мы устанавливаем символ '-' в соответствующую 
    }												                            		// позицию в векторе cave. Индекс строки задается outputAltitude - 2, чтобы не изменять вход и выход из лабиринта
  }
/*
	Этот метод соединяет последнюю строку лабиринта, чтобы обеспечить единственный путь к выходу. Он проходит по каждой ячейке в последней строке, 
	проверяет их значения вектора rowSet и соединяет только те ячейки, которые имеют разные значения
*/
  static void													                                		// Это объявление функции connectLastRow, которая соединяет 
  connectLastRow(std::shared_ptr<std::vector<std::vector<char>>> &cave,	  // последнюю строку лабиринта. Функция принимает указатель 
                 unsigned latitude, unsigned outputAltitude,		        	// на вектор векторов символов cave, ширину лабиринта, 
                 const std::vector<unsigned> &rowSet) {			            	// высоту вывода и вектор rowSet
    for (unsigned j = 0; j < latitude - 1; ++j) {		      // Это начало цикла, который проходит по каждому столбцу лабиринта в последней строке, за исключением последнего столбца
      if (rowSet[j] != rowSet[j + 1])					            // Эта строка проверяет, относятся ли два соседних столбца в последней строке к разным множествам 
        cave->at(outputAltitude - 2).at(j * 2 + 2) = ' '; // (заданы в векторе rowSet). Если это так, то мы соединяем эти два столбца символом '-'. 
    }													 	                          // Здесь мы устанавливаем символ '-' в соответствующую позицию в векторе cave. 
  }														                          	// Индекс строки задается outputAltitude - 2, чтобы не изменять вход и выход из лабиринта
};
