;Input bst: A binary search tree (possibly empty).
;Return: The integer key stored in bst's root node (or #f if bst is empty).
(define entry
  (lambda (bst)
    (car bst)))

;Input bst: A binary search tree (possibly empty).
;Return: The left subtree of bst's root node (or #f if bst is empty).
(define left
  (lambda (bst)
    (car (cdr bst))))

;Input bst: A binary search tree (possibly empty).
;Return: The right subtree of bst's root node (or #f if bst is empty).
(define right
  (lambda (bst)
    (car (cdr (cdr bst)))))

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
    ...))

;Input bst: A binary search tree (possibly empty).
;Return: A list of all entries in bst, ordered according to a inorder traversal.
(define inorder
  (lambda (bst)
    ...))

;Input bst: A binary search tree (possibly empty).
;Return: A list of all entries in bst, ordered according to a postorder traversal.
(define postorder
  (lambda (bst)
    ...))

;Input v: An integer.
;Input bst: A binary search tree (possibly empty).
;Return: A binary search tree, equal to the result of inserting v into bst. If bst
;already contained v, then this result is identical to bst.
(define insert
  (lambda (v bst)
    ...))