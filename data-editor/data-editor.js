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

  console.log(`static ${typeName} const ([\\S\\s\\r]+?^\\s+)(".+?")(.+\\/\\/ ${iniNameTag}[\\S\\s\\r]+?^\\s+)${ruleVaueCapture}(,.+\\/\\/\\s+?${ruleNameTag}[\\S\\s\\r]+?\\);[\\n\\r])`)

  const ruleRegex = new RegExp(
      `static ${typeName} const ([\\S\\s\\r]+?^\\s+)(".+?")(.+\\/\\/ ${iniNameTag}[\\S\\s\\r]+?^\\s+)${ruleVaueCapture}(,.+\\/\\/\\s+?${ruleNameTag}[\\S\\s\\r]+?\\);[\\n\\r])`,
      "mg"
  )

  console.log(`static ${typeName} const $1$2$3${ruleFunction}($2, $4)$5`)

  const rewrittenCode = sourceCode.replace(ruleRegex, `static ${typeName} const $1$2$3${ruleFunction}($2, $4)$5`)

  writeFileSync(sourceFile, rewrittenCode)
}

const BOOLEAN_RULE_CAPTURE = "(true|false)"
const INT_RULE_CAPTURE = "([-]*\\d+)"
const ARMOR_RULE_CAPTURE = "(ARMOR_.+?)"
const SPEED_RULE_CAPTURE = "(SPEED_.+?)"

const RULES = [
  ["Read_Can_Be_Found_In_Crate", BOOLEAN_RULE_CAPTURE, "Can this be a goodie surprise"],
  ["Read_Limited_Facings", BOOLEAN_RULE_CAPTURE, "Only has eight facings?"],
  ["Read_Can_Crush_Infantry", BOOLEAN_RULE_CAPTURE, "Can this unit squash infantry"],
  ["Read_Can_Harvest", BOOLEAN_RULE_CAPTURE, "Does this unit harvest Tiberium?"],
  ["Read_Radar_Equipped", BOOLEAN_RULE_CAPTURE, "Does it have a rotating radar dish?"],
  ["Read_Has_Fire_Animation", BOOLEAN_RULE_CAPTURE, "Is there an associated firing"],
  ["Read_Lock_Turret", BOOLEAN_RULE_CAPTURE, "Must the turret be in a locke"],
  ["Read_Has_Tracks", BOOLEAN_RULE_CAPTURE, "Does it lay tracks"],
  ["Read_Is_Huge", BOOLEAN_RULE_CAPTURE, "Is this a gigundo-rotun"],
  ["Read_Has_Constant_Animation", BOOLEAN_RULE_CAPTURE, "Does the unit have a constan"],
  ["Read_Unit_Speed", SPEED_RULE_CAPTURE, "MOVE:"]
]

function main() {
  RULES.forEach(i =>
    rewriteRule(
      "UDATA",
      "UnitTypeClass",
      i[0],
      "NAME:",
      i[1],
      i[2]
    )
  )
}

main()
