;Input nums: A list of numbers (possibly empty).
;Return: A number. The sum of the numbers in nums (0 if nums is empty).
(define sum
  (lambda (nums)
    (if (null? nums)
      0 
       (+ (car nums) sum (cdr nums)))))
