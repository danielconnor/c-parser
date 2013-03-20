var ast = [
    {
        "typeName": "Declaration",
        "declarations": [
            {
                "typeName": "Declarator",
                "variable": {
                    "typeName": "Variable",
                    "name": "s",
                    "type": {
                        "typeName": "Struct",
                        "name": "a",
                        "members": [
                            {
                                "typeName": "Variable",
                                "name": "a",
                                "type": {
                                    "typeName": "FundamentalType",
                                    "name": "int"
                                }
                            },
                            {
                                "typeName": "Variable",
                                "name": "b",
                                "type": {
                                    "typeName": "FundamentalType",
                                    "name": "int"
                                }
                            },
                            {
                                "typeName": "Variable",
                                "name": "c",
                                "type": {
                                    "typeName": "Array",
                                    "type": {
                                        "typeName": "FundamentalType",
                                        "name": "int"
                                    },
                                    "size": {
                                        "typeName": "NumberLiteral",
                                        "raw": "10",
                                        "intVal": 10,
                                        "doubleVal": -6.27744e+066
                                    }
                                }
                            }
                        ]
                    }
                },
                "init": null
            }
        ]
    },
    {
        "typeName": "Typedef",
        "type": {
            "typeName": "Struct",
            "name": "a",
            "members": [
                {
                    "typeName": "Variable",
                    "name": "a",
                    "type": {
                        "typeName": "FundamentalType",
                        "name": "int"
                    }
                },
                {
                    "typeName": "Variable",
                    "name": "b",
                    "type": {
                        "typeName": "FundamentalType",
                        "name": "int"
                    }
                },
                {
                    "typeName": "Variable",
                    "name": "c",
                    "type": {
                        "typeName": "Array",
                        "type": {
                            "typeName": "FundamentalType",
                            "name": "int"
                        },
                        "size": {
                            "typeName": "NumberLiteral",
                            "raw": "10",
                            "intVal": 10,
                            "doubleVal": -6.27744e+066
                        }
                    }
                }
            ]
        },
        "name": "d"
    },
    {
        "typeName": "FunctionPrototype",
        "returnType": {
            "typeName": "Pointer",
            "type": {
                "typeName": "FundamentalType",
                "name": "int"
            }
        },
        "name": "main",
        "arguments": [
            {
                "typeName": "Argument",
                "name": "",
                "type": {
                    "typeName": "Array",
                    "type": {
                        "typeName": "FundamentalType",
                        "name": "int"
                    },
                    "size": null
                }
            }
        ]
    },
    {
        "typeName": "FunctionDeclaration",
        "prototype": {
            "typeName": "FunctionPrototype",
            "returnType": {
                "typeName": "Pointer",
                "type": {
                    "typeName": "FundamentalType",
                    "name": "int"
                }
            },
            "name": "main",
            "arguments": [
                {
                    "typeName": "Argument",
                    "name": "b",
                    "type": {
                        "typeName": "Pointer",
                        "type": {
                            "typeName": "FundamentalType",
                            "name": "int"
                        }
                    }
                }
            ]
        },
        "body": {
            "typeName": "BlockStatement",
            "statements": [
                {
                    "typeName": "Declaration",
                    "declarations": [
                        {
                            "typeName": "Declarator",
                            "variable": {
                                "typeName": "Variable",
                                "name": "x",
                                "type": {
                                    "typeName": "Array",
                                    "type": {
                                        "typeName": "Struct",
                                        "name": "a",
                                        "members": [
                                            {
                                                "typeName": "Variable",
                                                "name": "a",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                }
                                            },
                                            {
                                                "typeName": "Variable",
                                                "name": "b",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                }
                                            },
                                            {
                                                "typeName": "Variable",
                                                "name": "c",
                                                "type": {
                                                    "typeName": "Array",
                                                    "type": {
                                                        "typeName": "FundamentalType",
                                                        "name": "int"
                                                    },
                                                    "size": {
                                                        "typeName": "NumberLiteral",
                                                        "raw": "10",
                                                        "intVal": 10,
                                                        "doubleVal": -6.27744e+066
                                                    }
                                                }
                                            }
                                        ]
                                    },
                                    "size": {
                                        "typeName": "NumberLiteral",
                                        "raw": "2",
                                        "intVal": 2,
                                        "doubleVal": -6.27744e+066
                                    }
                                }
                            },
                            "init": null
                        }
                    ]
                },
                {
                    "typeName": "Declaration",
                    "declarations": [
                        {
                            "typeName": "Declarator",
                            "variable": {
                                "typeName": "Variable",
                                "name": "y",
                                "type": {
                                    "typeName": "Struct",
                                    "name": "a",
                                    "members": [
                                        {
                                            "typeName": "Variable",
                                            "name": "a",
                                            "type": {
                                                "typeName": "FundamentalType",
                                                "name": "int"
                                            }
                                        },
                                        {
                                            "typeName": "Variable",
                                            "name": "b",
                                            "type": {
                                                "typeName": "FundamentalType",
                                                "name": "int"
                                            }
                                        },
                                        {
                                            "typeName": "Variable",
                                            "name": "c",
                                            "type": {
                                                "typeName": "Array",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                },
                                                "size": {
                                                    "typeName": "NumberLiteral",
                                                    "raw": "10",
                                                    "intVal": 10,
                                                    "doubleVal": -6.27744e+066
                                                }
                                            }
                                        }
                                    ]
                                }
                            },
                            "init": null
                        },
                        {
                            "typeName": "Declarator",
                            "variable": {
                                "typeName": "Variable",
                                "name": "z",
                                "type": {
                                    "typeName": "Array",
                                    "type": {
                                        "typeName": "Struct",
                                        "name": "a",
                                        "members": [
                                            {
                                                "typeName": "Variable",
                                                "name": "a",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                }
                                            },
                                            {
                                                "typeName": "Variable",
                                                "name": "b",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                }
                                            },
                                            {
                                                "typeName": "Variable",
                                                "name": "c",
                                                "type": {
                                                    "typeName": "Array",
                                                    "type": {
                                                        "typeName": "FundamentalType",
                                                        "name": "int"
                                                    },
                                                    "size": {
                                                        "typeName": "NumberLiteral",
                                                        "raw": "10",
                                                        "intVal": 10,
                                                        "doubleVal": -6.27744e+066
                                                    }
                                                }
                                            }
                                        ]
                                    },
                                    "size": {
                                        "typeName": "NumberLiteral",
                                        "raw": "43",
                                        "intVal": 43,
                                        "doubleVal": -6.27744e+066
                                    }
                                }
                            },
                            "init": null
                        }
                    ]
                },
                {
                    "typeName": "ExpressionStatement",
                    "expression": {
                        "typeName": "AssignExpression",
                        "op": "=",
                        "opName": "ASSIGN",
                        "left": {
                            "typeName": "MemberExpression",
                            "object": {
                                "typeName": "ArrayAccess",
                                "array": {
                                    "typeName": "Variable",
                                    "name": "x",
                                    "type": {
                                        "typeName": "Array",
                                        "type": {
                                            "typeName": "Struct",
                                            "name": "a",
                                            "members": [
                                                {
                                                    "typeName": "Variable",
                                                    "name": "a",
                                                    "type": {
                                                        "typeName": "FundamentalType",
                                                        "name": "int"
                                                    }
                                                },
                                                {
                                                    "typeName": "Variable",
                                                    "name": "b",
                                                    "type": {
                                                        "typeName": "FundamentalType",
                                                        "name": "int"
                                                    }
                                                },
                                                {
                                                    "typeName": "Variable",
                                                    "name": "c",
                                                    "type": {
                                                        "typeName": "Array",
                                                        "type": {
                                                            "typeName": "FundamentalType",
                                                            "name": "int"
                                                        },
                                                        "size": {
                                                            "typeName": "NumberLiteral",
                                                            "raw": "10",
                                                            "intVal": 10,
                                                            "doubleVal": -6.27744e+066
                                                        }
                                                    }
                                                }
                                            ]
                                        },
                                        "size": {
                                            "typeName": "NumberLiteral",
                                            "raw": "2",
                                            "intVal": 2,
                                            "doubleVal": -6.27744e+066
                                        }
                                    }
                                },
                                "index": {
                                    "typeName": "NumberLiteral",
                                    "raw": "0",
                                    "intVal": 0,
                                    "doubleVal": -6.27744e+066
                                }
                            },
                            "member": "a"
                        },
                        "right": {
                            "typeName": "AssignExpression",
                            "op": "=",
                            "opName": "ASSIGN",
                            "left": {
                                "typeName": "MemberExpression",
                                "object": {
                                    "typeName": "Variable",
                                    "name": "y",
                                    "type": {
                                        "typeName": "Struct",
                                        "name": "a",
                                        "members": [
                                            {
                                                "typeName": "Variable",
                                                "name": "a",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                }
                                            },
                                            {
                                                "typeName": "Variable",
                                                "name": "b",
                                                "type": {
                                                    "typeName": "FundamentalType",
                                                    "name": "int"
                                                }
                                            },
                                            {
                                                "typeName": "Variable",
                                                "name": "c",
                                                "type": {
                                                    "typeName": "Array",
                                                    "type": {
                                                        "typeName": "FundamentalType",
                                                        "name": "int"
                                                    },
                                                    "size": {
                                                        "typeName": "NumberLiteral",
                                                        "raw": "10",
                                                        "intVal": 10,
                                                        "doubleVal": -6.27744e+066
                                                    }
                                                }
                                            }
                                        ]
                                    }
                                },
                                "member": "a"
                            },
                            "right": {
                                "typeName": "NumberLiteral",
                                "raw": "5",
                                "intVal": 5,
                                "doubleVal": -6.27744e+066
                            }
                        }
                    }
                }
            ]
        }
    }
];