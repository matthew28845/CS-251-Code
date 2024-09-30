;Input start: An integer.
;Input stop: An integer.
;Return: A list of all integers that are >= start and <= stop, in order. If start > stop,
;then the list is empty.
(define gen-list
  (lambda (start stop)
    (if (> start stop) 
      '() 
	(cons start (gen-list (+ start 1) stop)))))

;Input lyst: A list of integers.
;Input desired: An integer.
;Return: A Boolean, which is #t if there are two consecutive integers in the list that
;add to the desired integer, and #f otherwise.
(define pair-sum?
  (lambda (lyst desired)
    (if (< (length lyst) 2) #f 
      (if (= (+ (car lyst) (car (cdr lyst))) desired) #t (pair-sum? (cdr lyst) desired)))))

;Input start: An integer.
;Input stop: An integer.
;Return: A lazy list of all integers that are >= start and <= stop, in order. If
;start > stop, then the list is empty.
(define gen-lazy-list
  (lambda (start stop)
    (if (> start stop)
      '()
      (cons start
        (lambda () (gen-lazy-list (+ start 1) stop))))))

;Input first: An integer.
;Return: A lazy list consisting of all integers >= first.
(define inf-seq
  (lambda (first)
    (gen-lazy-list 0 first)))

;Input lyst: An ordinary list.
;Return: A lazy list consisting of the same elements in the same order.
(define make-lazy
  (lambda (lyst)
    (if (null? (car lyst)) '() (cons (car lyst) (lambda () (make-lazy (cdr lyst)))))))

;Input lazy-list: A lazy list of integers.
;Input desired: An integer.
;Return: A Boolean, which is #t if there are two consecutive integers in the
;list that add to the desired integer, and #f otherwise.
(define pair-sum-lazy?
  (lambda (lazy-list desired)
    (if (< (length lazy-list) 2) #f 
      (if (= (+ (car lazy-list) 
		(car (cdr lazy-list))) desired) #t (lambda () (pair-sum-lazy? (cdr lazy-list) desired))))))

;Input lazy-list: A lazy list.
;Input n: An integer >= 1.
;Return: An ordinary list consisting of the first n elements of the given lazy list. If
;the lazy list contains fewer than n elements, then all of the values in the lazy list
;are returned. If the lazy list is empty, then so is the returned list.
(define first-n
  (lambda (lazy-list n)
    (if (< n 0) '() (cons (car lazy-list) (lambda () (first-n (cdr lazy-list) (- n 1)))))))

;Input lazy-list: A lazy list.
;Input n: An integer >= 1.
;Return: The nth element of the lazy list, indexing from 1. If there are fewer than n
;elements in the lazy list, then #f is returned.
(define nth
  (lambda (lazy-list n)
    (if (< n 0) #f (if (= n 0) (car lazy-list) (nth (cdr lazy-list) n)))))

;Input lazy-list: A lazy list of integers.
;Input n: An integer >= 2.
;Return: A lazy list, which is just like the given lazy list, except that all integer
;multiples of n have been removed. Warning: Unavoidably fails in certain situations where
;the result is supposed to be empty.
(define filter-multiples
  (lambda (lazy-list n)
    #f))
