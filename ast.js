var ast = [
    {
        "typeName": "StructStatement",
        "name": "thing",
        "members": [
            {
                "typeName": "Declaration",
                "type": {
                    "typeName": "Type",
                    "name": "int"
                },
                "declarations": [
                    {
                        "typeName": "Declarator",
                        "name": "i",
                        "init": null
                    },
                    {
                        "typeName": "Declarator",
                        "name": "j",
                        "init": null
                    }
                ]
            }
        ]
    },
    {
        "typeName": "Function",
        "returnType": {
            "typeName": "Type",
            "name": "double"
        },
        "name": "thing",
        "arguments": [
            {
                "typeName": "Argument",
                "name": "i",
                "type": {
                    "typeName": "Type",
                    "name": "int"
                }
            },
            {
                "typeName": "Argument",
                "name": "j",
                "type": {
                    "typeName": "Type",
                    "name": "int"
                }
            }
        ],
        "body": null
    },
    {
        "typeName": "Function",
        "returnType": {
            "typeName": "Type",
            "name": "signed int"
        },
        "name": "main",
        "arguments": [
        ],
        "body": {
            "typeName": "BlockStatement",
            "statements": [
                {
                    "typeName": "ExpressionStatement",
                    "expression": {
                        "typeName": "FunctionInvocation",
                        "arguments": [
                            {
                                "typeName": "NumberLiteral",
                                "raw": "1",
                                "value": 1
                            },
                            {
                                "typeName": "NumberLiteral",
                                "raw": "2",
                                "value": 2
                            },
                            {
                                "typeName": "NumberLiteral",
                                "raw": "43",
                                "value": 43
                            }
                        ],
                        "function": "thing"
                    }
                },
                {
                    "typeName": "Declaration",
                    "type": {
                        "typeName": "Type",
                        "name": "int"
                    },
                    "declarations": [
                        {
                            "typeName": "Declarator",
                            "name": "i",
                            "init": null
                        },
                        {
                            "typeName": "Declarator",
                            "name": "j",
                            "init": {
                                "typeName": "NumberLiteral",
                                "raw": "1",
                                "value": 1
                            }
                        }
                    ]
                },
                {
                    "typeName": "Declaration",
                    "type": {
                        "typeName": "Type",
                        "name": "char"
                    },
                    "declarations": [
                        {
                            "typeName": "Declarator",
                            "name": "a",
                            "init": {
                                "typeName": "Literal",
                                "raw": "a"
                            }
                        }
                    ]
                },
                {
                    "typeName": "ForStatement",
                    "condition": {
                        "typeName": "BinaryExpression",
                        "op": "<",
                        "opName": "LT",
                        "left": {
                            "typeName": "VariableExpression",
                            "raw": "i",
                            "type": {
                                "typeName": "Type",
                                "name": "int"
                            }
                        },
                        "right": {
                            "typeName": "NumberLiteral",
                            "raw": "10",
                            "value": 10
                        }
                    },
                    "body": {
                        "typeName": "BlockStatement",
                        "statements": [
                            {
                                "typeName": "IfStatement",
                                "condition": {
                                    "typeName": "BinaryExpression",
                                    "op": "==",
                                    "opName": "EQ",
                                    "left": {
                                        "typeName": "BinaryExpression",
                                        "op": "%",
                                        "opName": "MOD",
                                        "left": {
                                            "typeName": "VariableExpression",
                                            "raw": "i",
                                            "type": {
                                                "typeName": "Type",
                                                "name": "int"
                                            }
                                        },
                                        "right": {
                                            "typeName": "NumberLiteral",
                                            "raw": "2",
                                            "value": 2
                                        }
                                    },
                                    "right": {
                                        "typeName": "NumberLiteral",
                                        "raw": "0",
                                        "value": 0
                                    }
                                },
                                "body": {
                                    "typeName": "BlockStatement",
                                    "statements": [
                                        {
                                            "typeName": "Declaration",
                                            "type": {
                                                "typeName": "Type",
                                                "name": "int"
                                            },
                                            "declarations": [
                                                {
                                                    "typeName": "Declarator",
                                                    "name": "j",
                                                    "init": {
                                                        "typeName": "NumberLiteral",
                                                        "raw": "0",
                                                        "value": 0
                                                    }
                                                }
                                            ]
                                        },
                                        {
                                            "typeName": "WhileStatement",
                                            "condition": {
                                                "typeName": "BinaryExpression",
                                                "op": "<",
                                                "opName": "LT",
                                                "left": {
                                                    "typeName": "VariableExpression",
                                                    "raw": "j",
                                                    "type": {
                                                        "typeName": "Type",
                                                        "name": "int"
                                                    }
                                                },
                                                "right": {
                                                    "typeName": "NumberLiteral",
                                                    "raw": "10",
                                                    "value": 10
                                                }
                                            },
                                            "body": {
                                                "typeName": "BreakStatement"
                                            }
                                        },
                                        {
                                            "typeName": "ExpressionStatement",
                                            "expression": {
                                                "typeName": "AssignExpression",
                                                "op": "+=",
                                                "opName": "ADD_ASSIGN",
                                                "left": {
                                                    "typeName": "VariableExpression",
                                                    "raw": "j",
                                                    "type": {
                                                        "typeName": "Type",
                                                        "name": "int"
                                                    }
                                                },
                                                "right": {
                                                    "typeName": "VariableExpression",
                                                    "raw": "i",
                                                    "type": {
                                                        "typeName": "Type",
                                                        "name": "int"
                                                    }
                                                }
                                            }
                                        },
                                        {
                                            "typeName": "ExpressionStatement",
                                            "expression": {
                                                "typeName": "AssignExpression",
                                                "op": "+=",
                                                "opName": "ADD_ASSIGN",
                                                "left": {
                                                    "typeName": "VariableExpression",
                                                    "raw": "i",
                                                    "type": {
                                                        "typeName": "Type",
                                                        "name": "int"
                                                    }
                                                },
                                                "right": {
                                                    "typeName": "BinaryExpression",
                                                    "op": "+",
                                                    "opName": "ADD",
                                                    "left": {
                                                        "typeName": "VariableExpression",
                                                        "raw": "j",
                                                        "type": {
                                                            "typeName": "Type",
                                                            "name": "int"
                                                        }
                                                    },
                                                    "right": {
                                                        "typeName": "BinaryExpression",
                                                        "op": "+",
                                                        "opName": "ADD",
                                                        "left": {
                                                            "typeName": "NumberLiteral",
                                                            "raw": "1",
                                                            "value": 1
                                                        },
                                                        "right": {
                                                            "typeName": "BinaryExpression",
                                                            "op": "+",
                                                            "opName": "ADD",
                                                            "left": {
                                                                "typeName": "NumberLiteral",
                                                                "raw": "2",
                                                                "value": 2
                                                            },
                                                            "right": {
                                                                "typeName": "NumberLiteral",
                                                                "raw": "3",
                                                                "value": 3
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                    ]
                                },
                                "elseBody": {
                                    "typeName": "ExpressionStatement",
                                    "expression": {
                                        "typeName": "AssignExpression",
                                        "op": "+=",
                                        "opName": "ADD_ASSIGN",
                                        "left": {
                                            "typeName": "VariableExpression",
                                            "raw": "a",
                                            "type": {
                                                "typeName": "Type",
                                                "name": "char"
                                            }
                                        },
                                        "right": {
                                            "typeName": "NumberLiteral",
                                            "raw": "1",
                                            "value": 1
                                        }
                                    }
                                }
                            },
                            {
                                "typeName": "ForStatement",
                                "condition": null,
                                "body": {
                                    "typeName": "BlockStatement",
                                    "statements": [
                                        {
                                            "typeName": "ContinueStatement"
                                        }
                                    ]
                                },
                                "init": null,
                                "loop": null
                            }
                        ]
                    },
                    "init": {
                        "typeName": "AssignExpression",
                        "op": "=",
                        "opName": "ASSIGN",
                        "left": {
                            "typeName": "VariableExpression",
                            "raw": "i",
                            "type": {
                                "typeName": "Type",
                                "name": "int"
                            }
                        },
                        "right": {
                            "typeName": "BinaryExpression",
                            "op": "*",
                            "opName": "MUL",
                            "left": {
                                "typeName": "BinaryExpression",
                                "op": "+",
                                "opName": "ADD",
                                "left": {
                                    "typeName": "NumberLiteral",
                                    "raw": "1",
                                    "value": 1
                                },
                                "right": {
                                    "typeName": "NumberLiteral",
                                    "raw": "2",
                                    "value": 2
                                }
                            },
                            "right": {
                                "typeName": "NumberLiteral",
                                "raw": "2",
                                "value": 2
                            }
                        }
                    },
                    "loop": {
                        "typeName": "UpdateExpression",
                        "op": "++",
                        "opName": "INC",
                        "operand": {
                            "typeName": "VariableExpression",
                            "raw": "i",
                            "type": {
                                "typeName": "Type",
                                "name": "int"
                            }
                        },
                        "prefix": 0
                    }
                },
                {
                    "typeName": "ExpressionStatement",
                    "expression": {
                        "typeName": "AssignExpression",
                        "op": "=",
                        "opName": "ASSIGN",
                        "left": {
                            "typeName": "VariableExpression",
                            "raw": "i",
                            "type": {
                                "typeName": "Type",
                                "name": "int"
                            }
                        },
                        "right": {
                            "typeName": "BinaryExpression",
                            "op": "+",
                            "opName": "ADD",
                            "left": {
                                "typeName": "UpdateExpression",
                                "op": "++",
                                "opName": "INC",
                                "operand": {
                                    "typeName": "VariableExpression",
                                    "raw": "j",
                                    "type": {
                                        "typeName": "Type",
                                        "name": "int"
                                    }
                                },
                                "prefix": 0
                            },
                            "right": {
                                "typeName": "BinaryExpression",
                                "op": "+",
                                "opName": "ADD",
                                "left": {
                                    "typeName": "VariableExpression",
                                    "raw": "a",
                                    "type": {
                                        "typeName": "Type",
                                        "name": "char"
                                    }
                                },
                                "right": {
                                    "typeName": "NumberLiteral",
                                    "raw": "2",
                                    "value": 2
                                }
                            }
                        }
                    }
                },
                {
                    "typeName": "ReturnStatement",
                    "expression": {
                        "typeName": "NumberLiteral",
                        "raw": "0",
                        "value": 0
                    }
                }
            ]
        }
    }
];