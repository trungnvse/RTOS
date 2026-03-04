# 🛰️ ESP32 FreeRTOS Mastery Series

![FreeRTOS](https://img.shields.io/badge/RTOS-FreeRTOS-blueviolet?style=for-the-badge&logo=freertos)
![MCU](https://img.shields.io/badge/Hardware-ESP32-E7352C?style=for-the-badge&logo=espressif)

Chào mừng bạn đến với kho lưu trữ thực hành **FreeRTOS** trên dòng chip ESP32. Tài liệu này được biên soạn nhằm mục đích chia sẻ kiến thức về hệ điều hành thời gian thực thông qua các ví dụ code trực quan và thực tế.

## 📑 Danh mục bài học (Roadmap)

| Project | Concept Chính | Điểm nhấn kỹ thuật |
| :--- | :--- | :--- |
| **01** | **Binary Semaphore** | Đồng bộ Task từ ngắt (ISR) |
| **02** | **Task Communication** | Sử dụng biến `volatile` |
| **03** | **Counting Semaphore** | Quản lý đa tài nguyên (Multi-resources) |
| **04 & 05** | **Deadlock** | Phân tích lỗi treo hệ thống (Lethal errors) |
| **06** | **Priority Inheritance** | Cơ chế kế thừa ưu tiên trong Mutex |
| **07** | **Queue System** | Truyền dữ liệu cấu trúc (Struct) giữa các Task |
| **08** | **Software Timers** | Tối ưu hóa CPU không dùng Task |

---

## 🔍 Phân tích chi tiết từng Project

### 🟢 Project 01: Binary Semaphore & ISR Sync
Dự án này giải quyết bài toán: Làm sao để Task đọc cảm biến "thức dậy" ngay lập tức khi có tác động vật lý (nhấn nút)?

* **Logic ngắt (ISR):**
    ```cpp
    void IRAM_ATTR buttonInterruptHandler() {
      xSemaphoreGiveFromISR(xBinarySemaphore, &xWakeupHigherTask);
      if (xWakeupHigherTask == pdTRUE) portYIELD_FROM_ISR();
    }
    ```
> 💡 **Giải mã kỹ thuật từ tác giả:** > * `IRAM_ATTR`: Ép hàm vào RAM để thực thi tốc độ cao, tránh độ trễ khi đọc từ Flash.
> * `portYIELD_FROM_ISR`: Đây là "phép thuật" giúp hệ thống không chờ hết lượt (tick) mà chuyển sang Task ưu tiên cao hơn ngay khi thoát ngắt.



---

### 🟡 Project 03: Counting Semaphore (Quản lý bãi xe)
Khác với Binary Semaphore (chỉ có 0 và 1), Counting Semaphore cho phép quản lý một nhóm tài nguyên.
* **Kịch bản:** Bạn giới hạn 3 Semaphore cho 5 Task. 
* **Hiện tượng:** Bạn sẽ thấy trên Serial Log, 3 Task đầu sẽ chiếm dụng tài nguyên, Task 4 và 5 sẽ báo trạng thái `Waiting` cho đến khi có một Task "nhả" vé.

---

### 🔴 Project 04 & 05: Deadlock (Tử huyệt)
Đây là phần quan trọng nhất để hiểu về tính ổn định của hệ thống đa nhiệm.

* **Project 04 (Self-lock):** Một Task tự đợi chính mình trả khóa. 
* **Project 05 (Circular Wait):** Task 1 giữ A đợi B, Task 2 giữ B đợi A.
* **Dấu hiệu nhận biết:** Hệ thống ngừng in Log, mọi hoạt động bị đóng băng.



---

### 🟣 Project 06: Mutex & Priority Inheritance
Giải quyết vấn đề **Priority Inversion** (Đảo ngược ưu tiên) – một lỗi kinh điển trong RTOS.
* **Cơ chế:** Khi Task Cao bị chặn bởi Task Thấp đang giữ Mutex, FreeRTOS tự động nâng mức ưu tiên của Task Thấp lên. Điều này ngăn cản các Task Trung Bình chen ngang vào quá trình giải phóng tài nguyên.

---

### 🔵 Project 07: Queue (Hàng đợi cấu trúc)
Cách tốt nhất để truyền dữ liệu giữa các Task mà không lo ngại về xung đột bộ nhớ (Thread-safe).
* **Dữ liệu:** Sử dụng `SensorData_t` để đóng gói cả `temp` và `hum`.
* **Cơ chế:** Task gửi dữ liệu vào Queue, Task nhận sẽ đợi (Block) cho đến khi có dữ liệu mới xuất hiện.



---

### ⚪ Project 08: Software Timers
Thay vì tạo nhiều Task gây tốn RAM (Stack), ta sử dụng **Daemon Task** duy nhất của hệ thống để quản lý các tác vụ định kỳ.
* `pdTRUE`: Chế độ tự nạp lại (Auto-reload) - dùng cho LED nháy.
* `pdFALSE`: Chế độ chạy một lần (One-shot) - dùng cho các sự kiện timeout.

---

## 🛠 Yêu cầu hệ thống
* **Vi điều khiển:** ESP32 Dev Kit.
* **Cảm biến:** DHT11 (Chân 15), Nút nhấn (Chân 4).
* **Baudrate Monitor:** `115200`.

---
**THANK YOU!** 🙌  
*Tài liệu được đóng góp bởi **Trung Nguyễn**. Hy vọng nó giúp ích cho hành trình chinh phục Embedded System của bạn!*
