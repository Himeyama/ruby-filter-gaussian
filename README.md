# ruby-filter-gaussian
[![build](https://github.com/Himeyama/ruby-filter-gaussian/actions/workflows/build.yml/badge.svg)](https://github.com/Himeyama/ruby-filter-gaussian/actions/workflows/build.yml)
[![Gem Version](https://badge.fury.io/rb/gaussian-filter.svg)](https://badge.fury.io/rb/gaussian-filter)

## 概要
ガウシアンフィルターの Ruby 拡張。データが複数の場合は並列計算を行うため高速に計算されます。

基本的には、 https://docs.scipy.org/doc/scipy/reference/generated/scipy.ndimage.gaussian_filter1d.html 
と同じです。

- フィルターの標準偏差に対してデータが小さい場合は計算できません。
- 実装が異なるためか Python のライブラリと比較すると丸目誤差が存在する場合があります。誤差の大きさは 10^(-15) 程度です。

## インストール

GitHub から、`ruby-filter-gaussian` をインストールするには以下のコマンドを実行します。
```sh
gem install specific_install
gem specific_install -l himeyama/ruby-filter-gaussian
```


**Gemfile** に記述する場合は以下のようにして `bundle`。

```rb
gem "gaussian-filter", github: "himeyama/ruby-filter-gaussian.git", branch: :main
```

## 使用法
<dt><code>Gaussian.filter1d(ary, sd) -> Array</code></dt>
<dd>
    <p>データへガウシアンフィルター処理を行います。
    `ary` の大きさが1の場合、1次元配列が返ります。
    `ary` の大きさが2以上で `sd` が数値の場合は全てのデーターで `sd` が適応され、
    `sd` が配列の場合は各データのインデックスに対応します。
    </p>
    <dl>
        <dt>[PARAM] ary:</dt>
        <dd>一次元のデータ</dd>
        <dt>[PARAM] sd:</dt>
        <dd>ガウシアンフィルターの標準偏差。</dd>
    </dl>

```rb
require "Gaussian"

Gaussian.filter1d [1, 2, 3, 4, 5], 1
#=> [1.4270409503911738, 2.0678220347792573, 3.0, 3.932177965220743, 4.572959049608826]

Gaussian.filter1d [[1, 2, 3, 4, 5], [3, 2, 3, 4, 1]], 1
# => 
# [[1.4270409503911738, 2.0678220347792573, 3.0, 3.932177965220743, 4.572959049608826],
#  [2.708335457918112, 2.641484411955284, 2.8831540219185285, 2.757459057400815, 2.0095670508072625]]

Gaussian.filter1d [[1, 2, 3, 4, 5], [1, 2, 3, 4, 5]], [1, 0.5]
#=> 
# [[1.4270409503911738, 2.0678220347792573, 3.0, 3.932177965220743, 4.572959049608826],
#  [1.1072423672218035, 2.0002638650827373, 2.9999999999999996, 3.999736134917262, 4.892757632778196]]
```   

</dd>
