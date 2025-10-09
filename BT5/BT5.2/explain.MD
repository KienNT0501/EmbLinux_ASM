Câu hỏi: Điều gì xảy ra nếu không gọi lại alarm(1) trong hàm xử lý?
Khi không gọi hàm alarm(1) trong hàm xử lí, chương trình sẽ bị treo trong vòng while.
Vì bản chất của hàm alarm (1) nó sẽ gửi đi tín hiệu sau khoảng thời gian 1s khi được gọi. Do đó, nếu không gọi lại hàm alarm (1), sẽ không có tín hiệu nhận và không thực thi hàm xử lí.
Ngoài ra, hàm sleep (1) trong while loop cũng rất quan trọng, vì alarm (1) sẽ gửi đi tín hiệu sau mỗi 1s, tuy nhiên tốc độ loop của while loop là nhỏ hơn rất nhiều, do đó ta phải gọi sleep cho luồng để hàm alarm (1) kịp gửi  - nhận - xử lý và in ra terminal.