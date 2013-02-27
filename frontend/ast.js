var ast = [
    {
        "typeName": "FunctionDeclaration",
        "prototype": {
            "typeName": "FunctionPrototype",
            "returnType": {
                "typeName": "Type",
                "name": "int"
            },
            "name": "main",
            "arguments": [
                {
                    "typeName": "Argument",
                    "name": "a",
                    "type": {
                        "typeName": "Type",
                        "name": "int"
                    }
                }
            ]
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
                            "name": "b",
                            "init": {
                                "typeName": "NumberLiteral",
                                "raw": "10",
                                "intVal": 10,
                                "doubleVal": 4.94066e-323
                            }
                        }
                    ]
                },
                {
                    "typeName": "IfStatement",
                    "condition": {
                        "typeName": "VariableExpression",
                        "raw": "a",
                        "type": {
                            "typeName": "Type",
                            "name": "int"
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
                                        "name": "c",
                                        "init": {
                                            "typeName": "BinaryExpression",
                                            "op": "+",
                                            "opName": "ADD",
                                            "left": {
                                                "typeName": "VariableExpression",
                                                "raw": "a",
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
                                                    "raw": "b",
                                                    "type": {
                                                        "typeName": "Type",
                                                        "name": "int"
                                                    }
                                                },
                                                "right": {
                                                    "typeName": "UpdateExpression",
                                                    "op": "++",
                                                    "opName": "INC",
                                                    "operand": {
                                                        "typeName": "VariableExpression",
                                                        "raw": "b",
                                                        "type": {
                                                            "typeName": "Type",
                                                            "name": "int"
                                                        }
                                                    },
                                                    "prefix": 0
                                                }
                                            }
                                        }
                                    }
                                ]
                            }
                        ]
                    },
                    "elseBody": null
                }
            ]
        }
    }
];