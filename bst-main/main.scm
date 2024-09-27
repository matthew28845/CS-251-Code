;Input bst: A binary search tree (possibly empty).
;Return: The integer key stored in bst's root node (or #f if bst is empty).
(define entry
  (lambda (bst)
    (if (equal? (check-bst bst) #f) #f (car bst))))

;Input bst: A binary search tree (possibly empty).
;Return: The left subtree of bst's root node (or #f if bst is empty).
(define left
  (lambda (bst)
    (if (equal? (check-bst bst) #f) #f (car (cdr bst)))))

;Input bst: A binary search tree (possibly empty).
;Return: The right subtree of bst's root node (or #f if bst is empty).
(define right
  (lambda (bst)
    (if (equal? (check-bst bst) #f) #f (car (cdr (cdr bst))))))

;Input elt: An integer.
;Input left-bst: A binary search tree (possibly empty).
;Input right-bst: A binary search tree (possibly empty).
;Return: A binary search tree whose root contains elt as its key, and whose left and
;right subtrees are left-bst and right-bst.
(define make-bst
  (lambda (elt left-bst right-bst)
    (list elt left-bst right-bst)))

;Input bst: A binary search tree (possibly empty).
;Return: A list of all entries in bst, ordered according to a preorder traversal.
(define preorder
  (lambda (bst)
    (if (null? bst)
      '()
        (cons (entry bst) (append (preorder (left bst)) (preorder (right bst)))))))

;Input bst: A binary search tree (possibly empty).
;Return: A list of all entries in bst, ordered according to a inorder traversal.
(define inorder
  (lambda (bst)
    (if (null? bst)
      '()
      (append (inorder (left bst)) (cons (entry bst) (inorder (right bst)))))))

;Input bst: A binary search tree (possibly empty).
;Return: A list of all entries in bst, ordered according to a postorder traversal.
(define postorder
  (lambda (bst)
    (if (null? bst)
      '()
      (append (postorder (left bst)) (postorder (right bst)) (list (entry bst))))))

;Input v: An integer.
;Input bst: A binary search tree (possibly empty).
;Return: A binary search tree, equal to the result of inserting v into bst. If bst
;already contained v, then this result is identical to bst.
(define insert
  (lambda (v bst)
    (if (null? bst)
      (make-bst v '() '())
      (cond
        ((< v (entry bst))
          (make-bst (entry bst) (insert v (left bst)) (right bst)))
          ((> v (entry bst))
            (make-bst (entry bst) (left bst) (insert v (right bst))))
              (else bst)))))

;Input bst: A binary search tree (to validate.)
;Return: the bst if it is valid, #f if not
(define check-bst
  (lambda (bst)
    (if (integer? (car bst))
      (if (list? (car (cdr bst)))
        (if (list? (car (cdr (cdr bst))))
          #t
          #f) #f) (if (null? (car bst)) #t
                          ))))