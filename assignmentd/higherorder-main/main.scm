;Input f: A function of three arguments.
;Return: A function, which is the curried version of f. For example,
;((((curry3 f) x) y) z) is equivalent to (f x y z).
(define curry3
  (lambda (f)
    (lambda (x)
      (lambda (y)
        (lambda (z)
          (f x y z))))))

;Input c: A function, which is the curried version of some function of three arguments.
;Return: A function of three arguments, which is the uncurried version of c. For example,
;((uncurry3 (curry3 f)) x y z) is equivalent to (f x y z).
(define uncurry3
  (lambda (c)
    (lambda (x y z)
      (((c x) y) z))))

;Input pred: A function of one argument that returns a Boolean.
;Input lyst: A list.
;Return: A list consisting of all list elements for which pred returns #t.
(define my-filter
  (lambda (pred lyst)
    (if (null? lyst) (list)
      (if (equal? #t (pred (car lyst))) (append (list (car lyst)) (my-filter pred (cdr lyst))) (my-filter pred (cdr lyst))))))

;Input pred: A function of one argument that returns a Boolean.
;Input lyst: A list.
;Return: A Boolean: #t if any element of the list evaluates to #t under f, and #f if all
;elements of the list evaluate to #f under f.
(define exists
  (lambda (pred lyst)
    (if (null? lyst) #f
      (if (equal? #t (pred (car lyst))) #t (exists pred (cdr lyst))))))

;Input symb: A symbol.
;Input docs: A list of key-value pairs. A key is a symbol. A value is a list of symbols.
;Return: A list of all of the keys, such that symb is a member of the value associated to
;that key.
(define find-docs
  (lambda (symb docs)
    (map car 
	 (filter (lambda (doc) (exists (lambda (x) (equal? symb x)) (car (cdr doc)))) 
		 docs))))
