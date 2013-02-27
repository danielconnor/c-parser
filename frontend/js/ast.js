var ast ={
    "name": "main",
    "arguments": [
    ],
    "body": {
        "statements": [
            {
                "declarations": [
                    {
                    }
                ]
            },
            {
                "condition": {
                    "op": "<",
                    "opName": "LT",
                    "left": {
                        "raw": "i"
                    },
                    "right": {
                        "raw": "10"
                    }
                },
                "body": {
                    "statements": [
                        {
                            "condition": {
                                "op": "==",
                                "opName": "EQ",
                                "left": {
                                    "op": "%",
                                    "opName": "MOD",
                                    "left": {
                                        "raw": "i"
                                    },
                                    "right": {
                                        "raw": "2"
                                    }
                                },
                                "right": {
                                    "raw": "0"
                                }
                            },
                            "body": {
                                "statements": [
                                ]
                            }
                        }
                    ]
                },
                "init": {
                    "op": "=",
                    "opName": "ASSIGN",
                    "left": {
                        "raw": "i"
                    },
                    "right": {
                        "op": "+",
                        "opName": "ADD",
                        "left": {
                            "raw": "0"
                        },
                        "right": {
                            "op": "+",
                            "opName": "ADD",
                            "left": {
                                "raw": "1"
                            },
                            "right": {
                                "raw": "1"
                            }
                        }
                    }
                },
                "loop": {
                    "op": "+=",
                    "opName": "ADD_ASSIGN",
                    "left": {
                        "raw": "i"
                    },
                    "right": {
                        "raw": "1"
                    }
                }
            },
            {
                "expression": {
                    "raw": "0"
                }
            }
        ]
    }
}