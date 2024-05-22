# Sudoku Solve 

Dự án của sinh viên khoa _Công nghệ thông tin_ của trường _Đại học Công Nghệ - Đại học Quốc gia Hà Nội (UET)_ thực hiện trong môn học “Lập trình nâng cao” nhằm mục đích lấy điểm thành phần.

## **Mô tả**
- Tác giả: MSV: 23021653 - K68CA3 - Mạch Trần Quang Nhật.
- Dự án được xây dựng bằng **_ngôn ngữ C++_** và có sử dụng thêm **_thư viện SDL2_** (Simple DirectMedia Layer).
- Dự án được lấy cảm hứng từ trò chơi trí tuệ **_Sudoku_** phát minh bởi nhà toán học người Thụy Sỹ **_Leonhard Euler_** vào thế kỉ 18.
- Trong **_Sudoku Solve_**, người chơi sẽ là một **_Sudoku Solver_** giải những bài toán Sudoku với độ khó khác nhau, giúp rèn luyện trí não, phát triển kỹ năng logic, suy luận và tư duy phản biện.


## **Giới thiệu**

### **I. Giao diện ban đầu:**

*1. Biểu tượng game:*
   
![windowIcon](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266569_522858ba40ccab677a3bf59453ee7009.jpg)

- Lấy cảm ứng từ chữ “Sudoku” và cách điệu nó trên 1 ô vuông 3x3.

*2. Màn hình bắt đầu*
![startScreen](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266570_a9a65210f4eaa78fe0c668ec4bdb4ef5.jpg)
- Gồm 3 lựa chọn: `Start Game`, `Introduction`, `Exit`.

*a) Mục Introduction:*
![introduction1](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266568_474049d0543be2dc48ae15e017dbc6f6.jpg)
![introduction2](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266518_a118e46955c554ffe36cc8fbbe58f2b9.jpg)
- Giúp bạn có kiến thức nền tảng và nắm rõ luật chơi nếu là người lần đầu tiên chơi tựa game này.
  
*b) Mục Start Game:*
![startGame](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266482_3e280f19ade8d02687f56423e8a5684c.jpg).

- Bạn sẽ bắt tay vào việc giải câu đố với đề bài gồm những con số sẽ hiện lên trên ô vuông với 3 mức độ `Dễ`, `Trung bình`, `Khó` tùy thuộc vào năng lực của bạn.

### **II. Giao diện khi chơi:**

*1. Đề bài và Bảng chọn số.*
   
   ![question](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266481_98498f68e8a36e3b52ee2240270a6556.jpg)     ![number](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266441_f936fbcbd8f87cf185943357adc890dd.jpg)

- Chọn 1 ô còn trống trong **_Bảng đề_**, 1 hình tròn nhỏ xuất hiện hiện thị rằng bạn đã chọn ô đó.
- Click vào 1 số trong **_Bảng số_**, số vừa chọn sẽ hiện ra trên **_Bảng đề_** trong trường hợp bạn chọn đúng.

*2. Đám mây thông báo.*
   
   ![cloud](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824266439_579946fc6901cf94a133a71a125a9e63.jpg)
- Đám mây hiện ra thông báo ứng với mỗi câu trả lời bạn chọn.


*3. Bóng đèn gợi ý*
   
   ![hint](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824218232_bf2574215ba80f46e2e2c46b906f31c2.jpg)

- Sử dụng gợi ý nếu cần thiết. Tối đa 3 lần.

*4. Mạng*
   
   ![heart](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824218184_b5fb9516c37aff5740cc25281264b355.jpg)

- Bạn có thể mắc lỗi, nhưng đừng quá 3 nhé. Game Over đấy!!!

*5. Bật/ Tắt nhạc nền*
   
   ![music](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824218153_c0c5c73c05c0cf043bc1a12d8753e034.jpg)

- Bạn có thể tắt nhạc nền nếu nó ảnh hưởng đến sự tập trung của bạn.

*6. Trò chơi mới hoặc Từ bỏ cuộc chơi*

  ![newGame](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/Screenshot%202024-05-22%20204537.png)

  ![retire](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/Screenshot%202024-05-22%20204549.png)
  
  ![answer](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824218113_21b295a4ab1aab2fd4c9b85bfe18660d.jpg)
  
- Bắt đầu một ván mới bằng click `Trò chơi mới`.

- Lời giải sẽ hiện ra khi bạn click `Bỏ cuộc` nhé.

*7. Độ khó:*

  ![level](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824169635_3248493f5c7c3f19082b2480a8b41ccf.jpg)

- Chọn cấp độ phù hợp với bạn nhé: `Dễ`, `Trung bình`, `Khó`

*8. Các màn hình kết thúc*
   
   ![success](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824169751_6f950d0c3483a5f3fd04400464fb33a9.jpg)

- **_Giành chiến thắng_** khi bạn giải được câu đố.
  
   ![gameOver](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824169752_d5a51f35f79fb0a6e8921fc1d7bd74e7.jpg)
  
- **_Thất bại_** khi bạn đã dùng hết 3 **Mạng**.
  
   ![niceTry](https://github.com/quangnhat09z/Sudoku-Solve/blob/master/cmake-build-debug/MdImages/z5465824169740_7ee8f37fbe76806e614b7c3e35d00144.jpg)

- **_Bỏ cuộc_** khi bạn không thể giải nó trọn vẹn.
  
  Ở đây:
  
  _Nếu chọn `Trò chơi mới` thì bạn sẽ giải một câu đố khác với độ khó tương đương câu đố vừa giải.
  
  _Nếu chọn `Thoát` thì bạn sẽ quay về **màn hình bắt đầu**.
