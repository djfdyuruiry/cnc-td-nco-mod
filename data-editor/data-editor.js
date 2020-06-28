const { readFileSync, writeFileSync } = require("fs")
const { resolve } = require("path")

const SRC_PATH = resolve(`${__dirname}/../TIBERIANDAWN/`)

function rewriteRule(
  sourceFileName,
  typeName,
  ruleFunction,
  iniNameTag,
  ruleVaueCapture,
  ruleNameTag
) {
  const sourceFile = `${SRC_PATH}/${sourceFileName}.CPP`
  const sourceCode = readFileSync(sourceFile, "utf-8")

  const ruleRegex = new RegExp(
      `static ${typeName} const ([\\S\\s\\r]+?^\\s+)(".+?")(.+\\/\\/ ${iniNameTag}[\\S\\s\\r]+?^\\s+)${ruleVaueCapture}(,.+\\/\\/ ${ruleNameTag}[\\S\\s\\r]+?\\);[\\n\\r])`,
      "mg"
  )

  const rewrittenCode = sourceCode.replace(ruleRegex, `static ${typeName} const $1$2$3${ruleFunction}($2, $4)$5`)

  writeFileSync(sourceFile, rewrittenCode)
}

const BOOLEAN_RULE_CAPTURE = "(true|false)"
const INT_RULE_CAPTURE = "([-]*\\d+)"
const ARMOR_RULE_CAPTURE = "(ARMOR_.+?)"

const RULES = [
  ["Read_Is_Wall", BOOLEAN_RULE_CAPTURE, "Is this a wall type"],
  ["Read_Is_Factory", BOOLEAN_RULE_CAPTURE, "Is it a factory type"],
  ["Read_Use_Simple_Damage", BOOLEAN_RULE_CAPTURE, "Simple (one frame) damage"],
  ["Read_Is_Fixed_Speed", BOOLEAN_RULE_CAPTURE, "Animation rate is regulated"]
]

function main() {
  RULES.forEach(i =>
    rewriteRule(
      "BDATA",
      "BuildingTypeClass",
      i[0],
      "NAME:",
      i[1],
      i[2]
    )
  )
}

main()
