/**
 * Abstract Data Type (ADT) Specification: BlockSymbolTable
 * 
 * Overview:
 * The BlockSymbolTable is a data structure that maintains a symbol table for a 
 * a language with block structure. It allows for the insertion, retrieval, and 
 * deletion of identifiers (blocks) and their associated attributes within 
 * nested scopes (blocks). The innermost block represents the current scope, 
 * and identifiers defined in outer blocks are accessible unless shadowed by 
 * definitions in inner blocks.
 * 
 * Template Parameters
 * - typename Name: The type of the identifier (e.g., a string or integer).
 * - typename Attribute: The type of the attribute associated with the
 *                       identifier.
 * 
 * template <typename Name, Attribute>
 * class BlockSymbolTable
 * 
 * Methods
 * 
 * 1. Initialize
 * void Initialize(int size);
 * - Description: Creates an empty symbol table with an initial capacity
 *                specified by size.
 * - Parameters:
 *   - size: The initial capacity of the symbol table.
 * - Precondition: size must be a positive integer.
 * - Postcondition: The symbol table is empty and ready to use.
 * 
 * 2. EnterBlock
 * bool EnterBlock();
 * - Description: Indicates that a new block (scope) has been entered. This 
 *                typically corresponds to entering a new lexical scope in the 
 *                language (e.g., entering a function or a block of code).
 * - Returns: true if the operation was successful, false otherwise (e.g., if 
 *            the maximum number of nested blocks is exceeded).
 * - Postcondition: A new block is added to the symbol table, and it becomes the
 *                  innermost block.
 * 
 * 3. Add
 * void Add(Name identifier, Attribute attr);
 * - Description: Adds an identifier and its associated attribute to the current
 *                (innermost) block.
 * - Parameters:
 *   - identifier: The name of the identifier to be added.
 *   - attr: The attribute associated with the identifier.
 * - Precondition: The identifier must not already exist in the current block
 *                 (to avoid shadowing within the same block).
 * - Postcondition: The identifier and its attribute are added to the innermost 
 *                  block.
 * 
 * 4. LeaveBlock
 * void LeaveBlock();
 * - Description: Deletes all identifiers defined in the innermost block, 
 *                effectively leaving the current scope.
 * - Postcondition: The innermost block is removed, and all identifiers defined 
 *                  in that block are deleted. The next outer block becomes the
 *                  new innermost block.
 * 
 * 5. Retrieve
 * Attribute* Retrieve(Name identifier);
 * - Description: Retrieves the attribute of the most recently defined
 *                identifier with the given name, searching from the innermost 
 *                block outward.
 * - Parameters:
 *   - identifier: The name of the identifier to retrieve.
 * - Returns: A pointer to the attribute of the most recently defined 
 *            identifier, or nullptr if the identifier is not found in any
 *            block.
 * - Postcondition: The symbol table remains unchanged.
 * 
 * 6. IsInBlock
 * bool IsInBlock(Name identifier);
 * - Description: Checks if the given identifier is defined in the innermost
 *                block.
 * - Parameters:
 *   - identifier: The name of the identifier to check.
 * - Returns: true if the identifier is defined in the innermost block, false
 *            otherwise.
 * - Postcondition: The symbol table remains unchanged.
 */
