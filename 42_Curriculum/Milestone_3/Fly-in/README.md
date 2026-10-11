*This project has been created as part of the 42 curriculum by masanz-s*

[Martin GitHub](https://github.com/martinnsanzz)

# Fly-in
*Drones are interesting.*


## 📂 Description


## 🔷 Instructions


## ➕ Additional requirements

### 🐨 How it works?


## 📖 Resources
This is a list of multiple resources use through out the life-cycle of the project

### 🐍 Python
- [Pydantic vs Dataclass](https://medium.com/@nishthakukreti.01/pydantic-vs-dataclass-640ed78a5f7c)
- [Pydanctic Validation Doc's](https://pydantic.dev/docs/validation/latest/get-started/)

### 🧪 Pytest
[Pytest Docs](https://docs.pytest.org/en/stable/)

### 📋 Other Stuff



## 🤖 AI Usage

**AI was NOT used to generate code.** All function implementations were written by Martin™.

**Where AI was used:**


This README.md file was done by **human fingers, sweat and tears**. No AI wrote a single
line of text :)


# Working On
Currently working on Parsing.

# Things to do
Things that still need to be done from parsing:
- Hubs unique names (Check for duplicates in Drone Map)
- Check for hub valid integers. This includes (Start and end cant be equal, all other hubs must be between start and end they cant be outisde those boundaries, hubs cant have same position)
- Check connections links only contain valid hub names
- Connection cant have same link as others (a-b is the same as b-a. This is invalid)
- Start and end hubs ignore max_drones capacity (This is not an error, its ignored)

Test that still need to be written:
- Drone map class test suite
- All possible test for map_parser